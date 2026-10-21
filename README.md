# Hệ thống Quản lý Hàng tồn kho & Doanh thu Siêu thị (ĐT05)

Bài tập lớn môn **Cấu trúc dữ liệu và giải thuật**, Phân hiệu Trường Đại học Giao thông vận tải tại TP.HCM, năm học 2026–2027.

## 1. Giới thiệu

Chương trình C++ quản lý mặt hàng trong kho của một siêu thị, ghi nhận nhập/xuất kho, lập hóa đơn bán hàng và thống kê doanh thu. Dữ liệu mặt hàng được tổ chức bằng **cây cân bằng AVL** để tìm kiếm, thêm, xóa với độ phức tạp O(log n).

## 2. Chức năng

- Quản lý mặt hàng: thêm, sửa, xóa, tìm kiếm theo mã hàng
- Tra cứu theo khoảng (Range Search), ví dụ theo khoảng giá hoặc khoảng mã hàng
- Nhập kho, xuất kho; cảnh báo khi tồn kho xuống dưới mức tối thiểu
- Lập hóa đơn bán hàng (tự động trừ tồn kho)
- Thống kê doanh thu theo ngày, tháng, mặt hàng; danh sách mặt hàng bán chạy
- Lưu và đọc dữ liệu bằng file nhị phân

## 3. Cấu trúc dữ liệu và giải thuật

- Cây AVL: balance factor, 4 phép quay LL / RR / LR / RL
- Tìm kiếm trên cây, duyệt cây, Range Search
- Đọc/ghi file nhị phân

## 4. Cấu trúc thư mục

```
.
├── README.md
├── .gitignore
├── .github/          mẫu pull request
├── src/              mã nguồn .cpp (main.cpp, nghiệp vụ…)
├── include/          file tiêu đề .h (AVLTree.h, MatHang.h…)
├── data/             dữ liệu mẫu cho test và demo
├── tests/            chương trình kiểm thử
└── docs/             báo cáo LaTeX
```

## 5. Yêu cầu và cách biên dịch

Yêu cầu: trình biên dịch hỗ trợ C++17 (g++ 9 trở lên, hoặc Visual Studio 2019 trở lên).

Biên dịch bằng g++ (Linux, macOS, hoặc Windows với MinGW):

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o quanlykho
```

Chạy chương trình:

```bash
./quanlykho          # Linux, macOS
quanlykho.exe        # Windows
```

Nếu dùng Visual Studio hoặc Code::Blocks: tạo project mới, thêm các file trong `src/`, và thêm thư mục `include/` vào đường dẫn include của project.

## 6. Giao diện đã thống nhất

> Chốt sau buổi họp giao diện (19/10/2026).

- Cấu trúc `MatHang`: `maHang[16]`, `tenHang[64]`, `donGia` (long long, đồng), `soLuongTon`, `tonToiThieu`, `ngayNhap` (struct `Ngay`)
- Khóa của cây AVL: mã hàng (`maHang`), không cho trùng
- Tên hàm chính của `AVLTree<K, V>`: `insert`, `remove`, `search`, `rangeSearch`, `inorder`, `size`, `height`, `empty`, `clear`
- Tra cứu theo khoảng giá: phương án A, duyệt cây rồi lọc theo giá (O(n)), thực hiện ở module kho. Range Search của cây AVL dùng cho khoảng mã hàng (O(log n + k))
- Hóa đơn: `HoaDon` có tối đa 20 dòng `ChiTietHoaDon`
- Định dạng file nhị phân: xem mục 9

## 7. Thành viên

| TV | Họ tên | MSSV | Tài khoản GitHub | Phụ trách |
|---|---|---|---|---|
| 1 | Đoàn Quốc Nhật (nhóm trưởng) | 6651020059 | 6651020059 | Quản lý dự án, tích hợp, ghép báo cáo |
| 2 | Huỳnh Long Nhật | 6651020061 | longnhat76 | Cây AVL |
| 3 | Trần Văn Tài | 6651020077 | tranvantai76qn | Nghiệp vụ kho |
| 4 | Hoàng Xuân Nhuật  | 6651020062 | hoangxuannhat28-sudo | Doanh thu và đánh giá hiệu năng |
| 5 | Nguyễn Duy Khang | 6651020037 | duykhang47 | File, giao diện, demo |

Bảng công việc và tiến độ: (dán liên kết Jira vào đây)

## 8. Quy ước làm việc

- Không commit trực tiếp vào `main`; mỗi chức năng làm trên một nhánh riêng rồi mở pull request, có ít nhất 1 người review trước khi merge
- Tên nhánh: `feature/avl-core`, `feature/inventory`, `feature/revenue`, `feature/file-io`, `docs/chuong-3`…
- Mỗi người commit bằng tài khoản GitHub của mình
- Thông điệp commit rõ ràng, theo dạng `loại: mô tả`, ví dụ:
  - `feat: thêm phép quay LR cho cây AVL`
  - `fix: sửa lỗi xóa node có hai con`
  - `docs: viết nháp Chương 2`
  - `test: thêm test Range Search`

 
## 9. Định dạng file nhị phân

Đặc tả phiên bản 1 (T44). Giữ nguyên các quyết định đã chốt: hai file riêng, header chung `FileHeader`, ghi nguyên struct.

### 9.1 Hai file dữ liệu

| File | Magic | Nội dung |
|---|---|---|
| `data/mathang.bin` | `"MHG1"` | 1 `FileHeader` + `soBanGhi` bản ghi `MatHang` |
| `data/hoadon.bin` | `"HDN1"` | 1 `FileHeader` + `soBanGhi` bản ghi `HoaDon` |

Không có dấu kết thúc file. Các bản ghi nằm liên tiếp ngay sau header.

### 9.2 FileHeader (12 byte)

```cpp
struct FileHeader {
    char magic[4];     // "MHG1" hoặc "HDN1" (4 ký tự, không có '\0')
    int  version;      // hiện tại là 1
    int  soBanGhi;     // số bản ghi phía sau, phải >= 0
};
```

| Offset | Kích thước | Trường |
|---|---|---|
| 0 | 4 | magic |
| 4 | 4 | version |
| 8 | 4 | soBanGhi |

### 9.3 Bố cục bản ghi (đo trên g++ 64-bit, Linux)

Struct được ghi nguyên khối bằng `fwrite`, nên **kích thước file tính theo `sizeof`, đã gồm byte đệm (padding) do trình biên dịch chèn vào**.

**MatHang: `sizeof` = 112 byte**

| Offset | Kích thước | Trường |
|---|---|---|
| 0 | 16 | `char maHang[16]` |
| 16 | 64 | `char tenHang[64]` |
| 80 | 8 | `long long donGia` |
| 88 | 4 | `int soLuongTon` |
| 92 | 4 | `int tonToiThieu` |
| 96 | 12 | `Ngay ngayNhap` (3 số `int`: ngay, thang, nam) |
| 108 | 4 | đệm (padding) |

**HoaDon: `sizeof` = 688 byte** (luôn đủ chỗ cho 20 dòng, kể cả dòng chưa dùng)

| Offset | Kích thước | Trường |
|---|---|---|
| 0 | 16 | `char maHoaDon[16]` |
| 16 | 12 | `Ngay ngayLap` |
| 28 | 4 | đệm |
| 32 | 640 | `ChiTietHoaDon danhSachChiTiet[20]` (20 x 32 byte) |
| 672 | 4 | `int soLuongChiTiet` |
| 676 | 4 | đệm |
| 680 | 8 | `long long tongTien` |

**ChiTietHoaDon: `sizeof` = 32 byte**

| Offset | Kích thước | Trường |
|---|---|---|
| 0 | 16 | `char maHang[16]` |
| 16 | 4 | `int soLuongMua` |
| 20 | 4 | đệm |
| 24 | 8 | `long long donGia` |

### 9.4 Kích thước file kỳ vọng

```
mathang.bin = sizeof(FileHeader) + soBanGhi * sizeof(MatHang) = 12 + 112 * n
hoadon.bin  = sizeof(FileHeader) + soBanGhi * sizeof(HoaDon)  = 12 + 688 * m
```

Ví dụ: 50 mặt hàng cho `mathang.bin` dài 12 + 5600 = **5612 byte**. 10 hóa đơn cho `hoadon.bin` dài 12 + 6880 = **6892 byte**.

Trong code, luôn dùng `sizeof(...)`, không viết cứng các con số trên.

### 9.5 Quy tắc ghi

1. Mở file bằng `ios::binary`.
2. Ghi `FileHeader` rồi ghi từng bản ghi. Với mặt hàng, lấy từ `kho.inorder()` (thứ tự LNR, tăng dần theo `maHang`).
3. Ghi ra `<tên>.tmp` trước. Ghi xong và đóng file thành công thì đổi tên thành `<tên>.bin`, để file cũ không bị mất nếu chương trình dừng giữa chừng.
4. Thư mục `data/` phải tồn tại trước khi ghi.

### 9.6 Quy tắc đọc và kiểm tra

Kiểm tra **theo thứ tự**. Sai bước nào thì báo "file hỏng" (kèm lý do) và dừng, chương trình không được sập:

| Bước | Điều kiện | Thông báo gợi ý |
|---|---|---|
| 1 | Mở được file | "không mở được file" |
| 2 | Đọc đủ 12 byte header | "thiếu header" |
| 3 | `magic` đúng (`MHG1` / `HDN1`) | "sai định dạng file" |
| 4 | `version == 1` | "phiên bản không hỗ trợ" |
| 5 | `soBanGhi >= 0` | "số bản ghi không hợp lệ" |
| 6 | Kích thước file == `12 + soBanGhi * sizeof(bản ghi)` (tính bằng `long long`) | "kích thước file không khớp" |

Bổ sung khi đọc từng bản ghi:

- Ép byte cuối của `maHang`, `tenHang`, `maHoaDon` thành `'\0'` để chuỗi bị hỏng không gây đọc tràn.
- Hóa đơn: `soLuongChiTiet` phải nằm trong 0..20.
- Mặt hàng: `insert` trả `false` (trùng mã) thì bỏ qua bản ghi đó và đếm số bản ghi bị bỏ.

Nguyên tắc: đọc vào bộ nhớ tạm, kiểm tra xong hết mới nạp vào cây AVL (`kho.clear()` rồi `insert`) hoặc `vector<HoaDon>`. Nếu có lỗi, dữ liệu đang chạy giữ nguyên.

### 9.7 Giới hạn và thay đổi phiên bản

- File chỉ đảm bảo đọc được khi **cùng trình biên dịch và hệ điều hành** (kích thước và padding của struct có thể khác giữa các môi trường). Đặt trong `file_io.cpp`:
  ```cpp
  static_assert(sizeof(FileHeader) == 12 && sizeof(MatHang) == 112 &&
                sizeof(ChiTietHoaDon) == 32 && sizeof(HoaDon) == 688,
                "Kich thuoc struct khac dac ta file nhi phan (muc 9)");
  ```
  Nếu một thành viên biên dịch ra số khác (ví dụ MinGW 32-bit), lỗi hiện ngay lúc biên dịch.
- Khi thay đổi struct `MatHang`, `HoaDon`, `ChiTietHoaDon` hoặc `Ngay`, phải tăng `version`, đổi magic nếu cần, và cập nhật mục này. Bản đọc từ chối mọi phiên bản chưa biết.

### 9.8 Hàm đề xuất (`include/file_io.h`)

```cpp
bool luuMatHang(const std::string& duongDan, const AVLTree<std::string, MatHang>& kho);
bool luuHoaDon(const std::string& duongDan, const std::vector<HoaDon>& ds);

bool docMatHang(const std::string& duongDan, AVLTree<std::string, MatHang>& kho,
                std::string& thongBaoLoi);
bool docHoaDon(const std::string& duongDan, std::vector<HoaDon>& ds,
               std::string& thongBaoLoi);
// Hàm đọc trả false khi file hỏng; dữ liệu đang có trong kho / ds giữ nguyên.
```
