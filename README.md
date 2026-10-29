# ĐỀ TÀI ĐT05: HỆ THỐNG QUẢN LÝ HÀNG TỒN KHO & DOANH THU SIÊU THỊ (CÂY CÂN BẰNG AVL)

Bài tập lớn môn **Cấu trúc Dữ liệu và Giải thuật** – Cài đặt cây nhị phân tìm kiếm tự cân bằng `AVLTree<K, V>` ứng dụng quản lý kho hàng, lập hóa đơn bán hàng, thống kê doanh thu và đánh giá hiệu năng thực nghiệm.

---

## 1. Phân công nhiệm vụ và sản phẩm của 5 thành viên

| Thành viên | Nhánh Git (Feature Branch) | Các file phụ trách chính | Mã Task (Jira) |
|---|---|---|---|
| **TV1 (Nhóm trưởng)** | `main` / `feature/main-integration` | `README.md`, `Makefile`, `src/main.cpp`, `include/ngay.h`, `docs/baocao/main.tex`, `docs/baocao/loimoidau_ketluan.tex` | T01 – T15 |
| **TV2 (Cây AVL)** | `feature/avl-tree` | `include/AVLTree.h`, `include/AVLTree.tpp`, `tests/T23_test_suite.cpp`, `docs/baocao/chuong2_avl.tex`, `docs/baocao/T25_ra_soat_loi_AVL.md` | T16 – T25 |
| **TV3 (Quản lý Kho)** | `feature/module-kho` | `include/mathang.h`, `include/AVLTree_Stub.h`, `include/quan_ly_kho.h`, `src/quan_ly_kho.cpp`, `data/du_lieu_mau.csv`, `tests/test_kho.cpp`, `docs/baocao/chuong1_kho.tex`, `docs/baocao/chuong3_kho.tex` | T26 – T34 |
| **TV4 (Hóa đơn & Doanh thu)** | `feature/module-doanh-thu` | `include/hoadon.h`, `include/lap_hoa_don.h`, `src/lap_hoa_don.cpp`, `include/thongkedoanhthu.h`, `src/thongkedoanhthu.cpp`, `include/thongketop.h`, `include/Chuongtrinhsinhvadodulieu.h`, `src/benchmark.cpp`, `docs/baocao/chuong3_doanhthu.tex`, `docs/baocao/chuong4_benchmark.tex` | T35 – T43 |
| **TV5 (File nhị phân & Giao diện)** | `feature/fileio-ui` | `include/FileIO.h`, `tests/test_fileio.cpp`, `docs/baocao/chuong3_file_giaodien.tex`, `docs/demo_slide/T50_Slide_TomTat.md`, `docs/demo_slide/T51_KichBan_Demo.md` | T44 – T52 |

---

## 2. Biên bản chốt giao diện & Đặc tả File nhị phân (Task T03 & T44)

1. **Khóa chính trên cây AVL (`K`):** `std::string` (`maHang` tối đa 15 ký tự, không cho phép trùng khóa).
2. **Cấu trúc `MatHang` (`V`):**
   - `char maHang[16]`, `char tenHang[64]`, `long long donGia`, `int soLuongTon`, `int tonToiThieu`, `Ngay ngayNhap`.
3. **Chữ ký 9 hàm công khai của `AVLTree<K, V>`:**
   - `bool insert(const K& key, const V& value);`
   - `bool remove(const K& key);`
   - `V* search(const K& key) const;`
   - `void rangeSearch(const K& lo, const K& hi, std::vector<V>& result) const;`
   - `std::vector<V> inorder() const;`
   - `int size() const;`, `int height() const;`, `bool empty() const;`, `void clear();`
4. **Định dạng File nhị phân (`kho.dat`):**
   - Bố cục tuần tự bản ghi độ dài cố định: `[FileHeader (24 bytes)][MatHang x soMatHang][HoaDon x soHoaDon]`.
   - `FileHeader`: `magic[4] = "SMK1"`, `version = 1`, `kichThuocMH = sizeof(MatHang)`, `kichThuocHD = sizeof(HoaDon)`, `soMatHang`, `soHoaDon`.
   - Ghi an toàn qua file tạm `kho.dat.tmp` rồi `rename` sang `kho.dat`.

---

## 3. Cấu trúc thư mục

```
.github/    mẫu pull request
data/       du_lieu_mau.csv (50 mặt hàng mẫu)
docs/       baocao/ (LaTeX, ghi chú), demo_slide/
include/    toàn bộ file .h / .tpp
src/        main.cpp, quan_ly_kho.cpp, lap_hoa_don.cpp, thongkedoanhthu.cpp, benchmark.cpp
tests/      T23_test_suite.cpp, test_fileio.cpp, test_kho.cpp
Makefile
README.md
```

## 4. Hướng dẫn Biên dịch và Chạy chương trình

Chạy tất cả lệnh từ **thư mục gốc của repo** (để chương trình tìm thấy `data/du_lieu_mau.csv`).

### Cách 1: Dùng `make` (Linux / macOS / MinGW)
```bash
make            # biên dịch main_app và benchmark_app
./main_app      # chạy chương trình chính
./benchmark_app # chạy benchmark độc lập (Task T40)
make test       # chạy toàn bộ unit test và integration test
```

### Cách 2: Dùng `g++` trực tiếp (Windows / Dev-C++)
```bash
g++ -std=c++11 -Wall -Wextra -O2 -Iinclude src/main.cpp src/quan_ly_kho.cpp src/lap_hoa_don.cpp src/thongkedoanhthu.cpp -o main_app
g++ -std=c++11 -Wall -Wextra -O2 -Iinclude src/benchmark.cpp -o benchmark_app
g++ -std=c++11 -Wall -Iinclude tests/T23_test_suite.cpp -o test_suite
g++ -std=c++11 -Wall -Iinclude tests/test_fileio.cpp -o test_fileio
g++ -std=c++11 -Wall -Iinclude tests/test_kho.cpp src/quan_ly_kho.cpp -o test_kho
```
