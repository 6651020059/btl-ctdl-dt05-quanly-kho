# ĐỀ TÀI ĐT05: HỆ THỐNG QUẢN LÝ HÀNG TỒN KHO & DOANH THU SIÊU THỊ (CÂY CÂN BẰNG AVL)

Bài tập lớn môn **Cấu trúc Dữ liệu và Giải thuật** – Cài đặt cây nhị phân tìm kiếm tự cân bằng `AVLTree<K, V>` ứng dụng quản lý kho hàng, lập hóa đơn bán hàng, thống kê doanh thu và đánh giá hiệu năng thực nghiệm.

---

## 1. Phân công nhiệm vụ và sản phẩm của 5 thành viên

| Thành viên | Họ và tên – MSSV | Nhánh Git (Feature Branch) | Các file phụ trách chính | Mã Task (Jira) | Đóng góp |
|---|---|---|---|---|---|
| **TV1 (Nhóm trưởng)** | **Đoàn Quốc Nhật** (`6651020059`) | `main` / `feature/main-integration` | `README.md`, `Makefile`, `include/ngay.h`, `src/main.cpp`, `tests/test_khoi_dong.sh`, `docs/BienBan_T03_ChotGiaoDien.md`, `docs/baocao/main.tex`, `loimoidau.tex`, `ketluan.tex`, `phuluc_phancong.tex` | T01 – T15 | Chưa xác minh từ commit/Jira |
| **TV2 (Cây AVL)** | **Huỳnh Long Nhật** (`6651020061`) | `feature/avl-tree` | `include/AVLTree.h`, `include/AVLTree.tpp`, `tests/T23_test_suite.cpp`, `docs/baocao/chuong2_avl.tex`, `docs/baocao/T25_ra_soat_loi_AVL.md` | T16 – T25 | Chưa xác minh từ commit/Jira |
| **TV3 (Quản lý Kho)** | **Trần Văn Tài** (`6651020077`) | `feature/module-kho` | `include/mathang.h`, `include/quan_ly_kho.h`, `src/quan_ly_kho.cpp`, `data/du_lieu_mau.csv`, `tests/test_kho.cpp`, `docs/baocao/chuong1_kho.tex`, `chuong3_kho.tex`, `report.pdf` | T26 – T34 | Chưa xác minh từ commit/Jira |
| **TV4 (Hóa đơn & Doanh thu)** | **Hoàng Xuân Nhuật** (`6651020062`) | `feature/module-doanh-thu` | `include/hoadon.h`, `include/lap_hoa_don.h`, `src/lap_hoa_don.cpp`, `include/thongkedoanhthu.h`, `src/thongkedoanhthu.cpp`, `include/thongketop.h`, `include/Chuongtrinhsinhvadodulieu.h`, `src/benchmark.cpp`, `docs/baocao/chuong3_doanhthu.tex`, `chuong4_benchmark.tex` | T35 – T43 | Chưa xác minh từ commit/Jira |
| **TV5 (File nhị phân & Demo)** | **Nguyễn Duy Khang** (`6651020037`) | `feature/fileio-ui` | `include/FileIO.h`, `tests/test_fileio.cpp`, `docs/baocao/chuong3_file_giaodien.tex`, `docs/demo_slide/T50_Slide_BaoVe_DT05.pptx`, `T50_Slide_TomTat.md`, `T51_KichBan_Demo.md` | T44 – T52 | Chưa xác minh từ commit/Jira |

---

## 2. Biên bản chốt giao diện & Đặc tả File nhị phân (Task T03 & T44)

1. **Khóa chính trên cây AVL (`K`):** `std::string` (`maHang` tối đa 15 ký tự, không cho phép trùng khóa).
2. **Cấu trúc `MatHang` (`V`):**
   - `char maHang[16]`, `char tenHang[64]`, `long long donGia`, `int soLuongTon`, `int tonToiThieu`, `Ngay ngayNhap`.
3. **Chữ ký 9 hàm công khai của `AVLTree<K, V>`:**
   - `bool insert(const K& key, const V& value);`
   - `bool remove(const K& key);`
   - `V* search(const K& key);` và `const V* search(const K& key) const;` (quyền sửa phụ thuộc constness của cây).
   - Hỗ trợ move constructor/move assignment; copy bị vô hiệu hóa để tránh sao chép cây ngoài ý muốn.
   - `void rangeSearch(const K& lo, const K& hi, std::vector<V>& result) const;`
   - `std::vector<V> inorder() const;`
   - `int size() const;`, `int height() const;`, `bool empty() const;`, `void clear();`
4. **Định dạng File nhị phân (`kho.dat`):**
   - Bố cục tuần tự bản ghi độ dài cố định: `[FileHeader (24 bytes)][MatHang x soMatHang][HoaDon x soHoaDon]`.
   - `FileHeader`: `magic[4] = "SMK1"`, `version = 1`, `kichThuocMH = sizeof(MatHang)`, `kichThuocHD = sizeof(HoaDon)`, `soMatHang`, `soHoaDon`.
   - Ghi an toàn qua file tạm `kho.dat.tmp` rồi `rename` sang `kho.dat`.
5. **Bảo vệ dữ liệu khi khởi động (`src/main.cpp`):** chương trình phân biệt 3 tình huống, không bao giờ tự ghi đè `kho.dat` của người dùng:
   - `kho.dat` đọc được → khôi phục kho và hóa đơn.
   - `kho.dat` chưa tồn tại → nạp 50 mặt hàng mẫu từ `data/du_lieu_mau.csv`.
   - `kho.dat` tồn tại nhưng hỏng/không đọc được (rỗng, bị cắt, sai định dạng, không có quyền đọc...) → báo lỗi kèm lý do và để người dùng chọn: **thoát giữ nguyên file**, hoặc **sao lưu thành `kho.dat.hongN`** rồi dùng dữ liệu mẫu (không bao giờ ghi đè bản sao lưu cũ; sao lưu thất bại thì dừng).
6. **Kiểm tra ngày theo lịch thực tế:** dùng chung hàm `ngayHopLe()` trong `include/ngay.h` (tháng 30/31 ngày, năm nhuận). 31/02, 31/04, 29/02 năm thường bị từ chối ở cả nhập kho, lập hóa đơn và thống kê.
7. **Chặn tràn số khi nhập kho:** `nhapKho()` so sánh `soLuongNhap > INT_MAX - soLuongTon` **trước** khi cộng; vượt giới hạn thì từ chối và giữ nguyên tồn kho.
8. **Lập hóa đơn theo giao dịch:** các dòng hàng được kiểm tra và gom trước; chỉ trừ tồn sau khi người dùng xác nhận toàn bộ. EOF/hủy giữa chừng không lưu hóa đơn và không thay đổi tồn.
9. **Header dùng chung:** alias `KhoHang` được khai báo một lần trong `include/khohang.h`; helper trừ tồn dùng chung nằm trong `include/nghiep_vu_ton.h`.

---

## 3. Hướng dẫn Biên dịch và Chạy chương trình

### Cách 1: Dùng `make` (khuyến nghị – Linux / macOS / Windows có MinGW)
```bash
make          # biên dịch main_app và benchmark_app
./main_app    # chạy chương trình chính (chạy từ thư mục gốc của repo để đọc data/du_lieu_mau.csv)
./benchmark_app   # chạy benchmark độc lập (Task T40)
make test     # biên dịch và chạy toàn bộ bộ kiểm thử
make asan     # chạy bộ kiểm thử với AddressSanitizer + UndefinedBehaviorSanitizer
make clean
```

### Cách 2: Dùng `g++` trực tiếp (Windows / Dev-C++ / Linux / macOS)
Chạy các lệnh sau tại thư mục gốc của repo:
```bash
# Biên dịch chương trình chính
g++ -std=c++11 -Wall -Wextra -O2 -Iinclude src/main.cpp src/quan_ly_kho.cpp src/lap_hoa_don.cpp src/thongkedoanhthu.cpp -o main_app

# Biên dịch chương trình Benchmark độc lập (Task T40)
g++ -std=c++11 -Wall -Wextra -O2 -Iinclude src/benchmark.cpp -o benchmark_app
```

### Cách 3: Chạy bộ kiểm thử tự động (Unit Test & Integration Test)
```bash
g++ -std=c++11 -Wall -Iinclude tests/T23_test_suite.cpp -o test_suite && ./test_suite
g++ -std=c++11 -Wall -Iinclude tests/test_fileio.cpp -o test_fileio && ./test_fileio
g++ -std=c++11 -Wall -Iinclude tests/test_kho.cpp src/quan_ly_kho.cpp -o test_kho && ./test_kho
g++ -std=c++11 -Wall -Iinclude tests/test_hoadon.cpp src/lap_hoa_don.cpp src/thongkedoanhthu.cpp -o test_hoadon && ./test_hoadon
make && sh tests/test_khoi_dong.sh    # kiểm thử luồng khởi động với kho.dat hỏng (chạy trong thư mục tạm)
```

> **An toàn dữ liệu:** các bài test chỉ dùng file tạm `tmp_*.dat` / `kho_test.dat`; `make test` và `make clean` **không xóa** `kho.dat` thật của người dùng.


> **Ghi chú xác minh:** Bảng trên là phân công theo kế hoạch, không phải tỷ lệ đóng góp đã được audit. Tỷ lệ thực tế, trạng thái PR/Jira, chia sẻ LaTeX cho giảng viên (T04), đọc chéo (T53) và tập dượt (T52) cần được xác nhận bằng bằng chứng tương ứng. Không suy ra trạng thái Done chỉ vì có file trong gói.
