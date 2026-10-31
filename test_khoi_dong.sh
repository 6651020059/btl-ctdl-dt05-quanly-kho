#!/bin/sh
# test_khoi_dong.sh - Kiểm thử luồng KHỞI ĐỘNG của main_app liên quan tới kho.dat (TV1, T06/T08).
# Mục tiêu: file kho.dat hỏng KHÔNG bao giờ bị tự động ghi đè bằng dữ liệu mẫu.
# Chạy trong thư mục tạm, không đụng tới kho.dat thật của người dùng.
# Cách chạy (từ thư mục gốc repo, sau khi `make`):  sh tests/test_khoi_dong.sh
ROOT=$(pwd)
EXE="$ROOT/main_app"
[ -x "$EXE" ] || { echo "Chua co main_app, hay chay make truoc"; exit 1; }
TMP=$(mktemp -d 2>/dev/null || echo "$ROOT/tmp_khoi_dong")
mkdir -p "$TMP"
trap 'rm -rf "$TMP"' EXIT
cp -r "$ROOT/data" "$TMP/data"
cp "$EXE" "$TMP/main_app"
cd "$TMP" || exit 1

DAT=0; TRUOT=0
ok()   { DAT=$((DAT+1));   echo "[OK]   $1"; }
fail() { TRUOT=$((TRUOT+1)); echo "[FAIL] $1"; }
kiem() { if [ "$1" = "0" ]; then ok "$2"; else fail "$2"; fi; }

# 1. Chưa có kho.dat -> nạp 50 mặt hàng mẫu, thoát thì tạo kho.dat
rm -f kho.dat*
printf '0\n' | ./main_app > out.txt 2>&1
grep -q "Chưa có 'kho.dat'" out.txt;  kiem $? "1a: chua co kho.dat -> nap du lieu mau"
[ -s kho.dat ];                        kiem $? "1b: thoat xong da tao kho.dat"

# 2. kho.dat chỉ 3 byte, chọn 0 (thoát) -> file giữ NGUYÊN
rm -f kho.dat*; printf 'abc' > kho.dat
printf '0\n' | ./main_app > out.txt 2>&1; RC=$?
grep -q "KHÔNG đọc được" out.txt;                 kiem $? "2a: bao file kho.dat bi hong"
[ "$RC" != "0" ];                                 kiem $? "2b: thoat voi ma loi khac 0"
[ "$(cat kho.dat)" = "abc" ] && [ "$(wc -c < kho.dat)" -eq 3 ]; kiem $? "2c: kho.dat van 3 byte, KHONG bi ghi de"
! grep -q "mặt hàng mẫu" out.txt;                 kiem $? "2d: khong tu dong nap du lieu mau"

# 3. kho.dat rỗng (0 byte), thoát ngay -> giữ nguyên
rm -f kho.dat*; : > kho.dat
printf '0\n' | ./main_app > out.txt 2>&1
[ ! -s kho.dat ] && [ -f kho.dat ];               kiem $? "3: kho.dat rong van con nguyen 0 byte"

# 4. Hỏng, chọn 1 -> sao lưu thành kho.dat.hong1 rồi mới dùng dữ liệu mẫu
rm -f kho.dat*; printf 'abc' > kho.dat
printf '1\n0\n' | ./main_app > out.txt 2>&1
[ "$(cat kho.dat.hong1)" = "abc" ];               kiem $? "4a: ban hong duoc sao luu day du (kho.dat.hong1)"
grep -q "Đã nạp 50 mặt hàng mẫu" out.txt;         kiem $? "4b: sau khi sao luu moi nap 50 mat hang mau"

# 5. Hỏng lần 2 -> không ghi đè bản sao lưu cũ (tạo hong2)
printf 'xyz' > kho.dat
printf '1\n0\n' | ./main_app > out.txt 2>&1
[ "$(cat kho.dat.hong1)" = "abc" ] && [ "$(cat kho.dat.hong2)" = "xyz" ]; kiem $? "5: ban sao luu cu khong bi ghi de, tao kho.dat.hong2"

# 6. File hợp lệ -> khôi phục bình thường, không nạp lại mẫu
rm -f kho.dat*
printf '0\n' | ./main_app > /dev/null 2>&1
printf '0\n' | ./main_app > out.txt 2>&1
grep -q "tự động khôi phục 50 mặt hàng" out.txt;  kiem $? "6: kho.dat hop le duoc khoi phuc (50 mat hang)"

# 7. Cắt ngắn file hợp lệ -> phát hiện hỏng, không ghi đè
head -c 200 kho.dat > cut.tmp && cp cut.tmp kho.dat
printf '0\n' | ./main_app > out.txt 2>&1
[ "$(wc -c < kho.dat)" -eq 200 ];                 kiem $? "7: file bi cat ngan duoc giu nguyen 200 byte"

echo "TONG KET khoi dong: $DAT dat, $TRUOT truot"
[ "$TRUOT" -eq 0 ]
