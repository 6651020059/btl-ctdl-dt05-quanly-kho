# ĐT05 – Hệ thống quản lý hàng tồn kho và doanh thu siêu thị sử dụng cây AVL

Bài tập lớn học phần **Cấu trúc dữ liệu và Giải thuật** – Lớp **Kỹ thuật điện tử viễn thông K66**.

| Thông tin | Nội dung |
|---|---|
| Đề tài | Hệ thống quản lý hàng tồn kho và doanh thu siêu thị sử dụng cây cân bằng AVL |
| Nhóm | ĐT05 |
| Giảng viên hướng dẫn | Trần Thị Dung |
| Repository | <https://github.com/6651020059/btl-ctdl-dt05-quanly-kho> |

## 1. Giới thiệu đề tài

Dự án xây dựng chương trình quản lý hàng tồn kho và doanh thu siêu thị bằng **C++11**. Cấu trúc dữ liệu trung tâm là cây tìm kiếm nhị phân tự cân bằng **AVLTree<K, V>**, được cài đặt để quản lý danh mục mặt hàng và hỗ trợ tra cứu theo khóa.

Chương trình có giao diện dòng lệnh (console), phục vụ các nghiệp vụ chính: quản lý danh mục hàng hóa; nhập kho, xuất kho; cảnh báo tồn kho tối thiểu; lập hóa đơn bán hàng; thống kê doanh thu; lưu và khôi phục dữ liệu bằng file nhị phân. Dự án cũng có chương trình benchmark để so sánh thời gian tra cứu của AVL với phương pháp tìm kiếm trên vector đã sắp xếp.

**Phạm vi hiện tại:** đây là ứng dụng C++ chạy trên console, không phải ứng dụng web. Repository hiện chưa cung cấp frontend web, HTTP API, đăng nhập/phân quyền hoặc cơ sở dữ liệu máy chủ.

## 2. Chức năng chính

- **Cây AVL:** thêm, xóa, tìm kiếm, duyệt inorder và tra cứu theo khoảng khóa; tự cân bằng bằng các phép quay LL, RR, LR, RL.
- **Quản lý mặt hàng:** thêm, sửa, xóa, tìm kiếm theo mã; liệt kê danh mục và tra cứu theo phạm vi.
- **Quản lý kho:** nhập kho, xuất kho, kiểm tra số lượng hợp lệ và cảnh báo khi tồn kho chạm/vượt ngưỡng tối thiểu.
- **Bán hàng và hóa đơn:** lập hóa đơn, kiểm tra tồn trước khi bán, tính thành tiền và cập nhật lượng tồn sau giao dịch hợp lệ.
- **Thống kê:** tổng hợp doanh thu theo ngày/tháng và thống kê mặt hàng bán chạy.
- **Lưu trữ:** lưu/đọc dữ liệu kho và hóa đơn bằng file nhị phân `kho.dat`; có dữ liệu CSV mẫu để khởi tạo khi chưa có file dữ liệu lưu.
- **Benchmark:** đo thời gian tìm kiếm và thao tác trên các bộ dữ liệu thử nghiệm.

## 3. Công nghệ và yêu cầu môi trường

- C++11.
- GCC/G++ có hỗ trợ C++11.
- GNU Make.
- Bash/sh để chạy một số lệnh kiểm thử trong Makefile.
- XeLaTeX và TeX Live hoặc MiKTeX nếu cần biên dịch lại báo cáo LaTeX.

### Cài công cụ trên Ubuntu/Debian

Mở Terminal và chạy:

```bash
sudo apt update
sudo apt install -y build-essential make
```

`build-essential` cung cấp GCC/G++ và các công cụ biên dịch cần thiết. Nếu cần biên dịch báo cáo LaTeX, cài TeX Live có XeLaTeX (có thể cần bộ gói LaTeX bổ sung theo các package được khai báo trong `main.tex`).

### Trên Windows

Có thể sử dụng **WSL2 với Ubuntu**. Sau khi cài Ubuntu, chạy lệnh cài đặt ở trên. Cách này cung cấp môi trường GNU Make và shell tương thích với các lệnh trong Makefile. Nếu dùng môi trường Windows khác, cần bảo đảm đã cài G++/GNU Make và shell tương thích.

## 4. Tải mã nguồn

Clone repository:

```bash
git clone https://github.com/6651020059/btl-ctdl-dt05-quanly-kho.git
cd btl-ctdl-dt05-quanly-kho
```

Các lệnh build, chạy chương trình và kiểm thử bên dưới cần được thực hiện tại **thư mục gốc repository**.

## 5. Biên dịch chương trình C++

Xóa các sản phẩm build cũ (nếu có) và biên dịch các chương trình:

```bash
make clean
make all
```

Sau khi build thành công, thư mục gốc sẽ có hai chương trình:

- `main_app`: chương trình quản lý kho và doanh thu.
- `benchmark_app`: chương trình benchmark.

### Chạy chương trình chính

```bash
./main_app
```

Chạy từ thư mục gốc repository để chương trình tìm đúng đường dẫn dữ liệu tương đối. Khi chưa có `kho.dat` hợp lệ, chương trình có thể nạp dữ liệu mẫu từ `data/du_lieu_mau.csv` theo luồng khởi tạo được cài đặt.

### Chạy benchmark

```bash
./benchmark_app
```

Kết quả benchmark phụ thuộc môi trường phần cứng, trình biên dịch, cờ tối ưu và dữ liệu đầu vào; khi báo cáo số liệu cần ghi lại môi trường chạy tương ứng.

## 6. Chạy kiểm thử

Chạy toàn bộ bộ kiểm thử:

```bash
make test
```

Chạy lại các kiểm thử với AddressSanitizer và UndefinedBehaviorSanitizer:

```bash
make asan
```

`make asan` sẽ thực hiện clean và build/test với các cờ sanitizer. Nếu cần chạy lại bản build thông thường sau đó, dùng:

```bash
make clean
make all
```

Lệnh dọn sản phẩm build:

```bash
make clean
```

**Lưu ý dữ liệu:** `kho.dat` là dữ liệu nhị phân phát sinh khi sử dụng chương trình. Không xóa hoặc ghi đè file này nếu đang chứa dữ liệu cần giữ. Các lệnh kiểm thử dùng file tạm; Makefile được thiết kế để dọn các file kiểm thử mà không chủ động xóa `kho.dat` của người dùng.

## 7. Cấu trúc repository

```text
.
├── include/                  # Header: AVL, mặt hàng, kho, hóa đơn, FileIO...
├── src/                      # Chương trình chính, quản lý kho, hóa đơn, doanh thu, benchmark
├── tests/                    # Unit test và kiểm thử tích hợp/khởi động
├── data/
│   └── du_lieu_mau.csv       # Dữ liệu mẫu
├── docs/
│   ├── baocao/               # Nguồn LaTeX, PDF và script biên dịch báo cáo
│   ├── demo_slide/           # Slide và kịch bản demo
│   ├── verification/         # Ghi nhận kiểm chứng kỹ thuật
│   └── KeHoach_BTL_DT05_ChiTiet.xlsx
├── .github/workflows/        # GitHub Actions cho build và test C++
├── Makefile
└── README.md
```

## 8. Các module mã nguồn chính

| Tệp | Vai trò |
|---|---|
| `include/AVLTree.h`, `include/AVLTree.tpp` | Cài đặt cây AVL tổng quát và các thao tác trên cây |
| `include/mathang.h`, `include/khohang.h` | Cấu trúc mặt hàng và kiểu dữ liệu kho |
| `src/quan_ly_kho.cpp` | Nghiệp vụ CRUD, nhập/xuất và cảnh báo tồn kho |
| `include/hoadon.h`, `src/lap_hoa_don.cpp` | Dữ liệu và nghiệp vụ lập hóa đơn |
| `src/thongkedoanhthu.cpp`, `include/thongketop.h` | Thống kê doanh thu và mặt hàng bán chạy |
| `include/FileIO.h` | Lưu/đọc dữ liệu nhị phân |
| `src/main.cpp` | Menu và luồng điều phối chương trình |
| `src/benchmark.cpp` | Chương trình đo hiệu năng |

## 9. Biên dịch báo cáo LaTeX

Nguồn báo cáo nằm trong `docs/baocao/`. Cần cài XeLaTeX và các package LaTeX mà tài liệu sử dụng.

**Ubuntu/Linux/macOS (TeX Live):**

```bash
cd docs/baocao
chmod +x build_report.sh
./build_report.sh
```

**Windows (MiKTeX hoặc TeX Live):**

Mở Command Prompt tại thư mục `docs/baocao/` và chạy:

```bat
build_report.bat
```

Các script gọi XeLaTeX ba lượt để cập nhật mục lục, tham chiếu và số trang; sau đó sao chép `main.pdf` thành `report.pdf`.

## 10. Kiểm tra tự động trên GitHub

Workflow tại `.github/workflows/cpp-ci.yml` được cấu hình chạy khi có `push` hoặc `pull_request`. Workflow cài công cụ build trên Ubuntu, sau đó chạy:

```bash
make clean all
make test
```

Có thể xem trạng thái chạy tại thẻ **Actions** của repository trên GitHub.

## 11. Phân công thành viên

Phạm vi phân công dưới đây phản ánh kế hoạch công việc của nhóm; không phải tỷ lệ phần trăm đóng góp đã được đo lường.

| Thành viên | Phạm vi phụ trách |
|---|---|
| **TV1 – Đoàn Quốc Nhật (nhóm trưởng)** | Điều phối, tích hợp mã nguồn, chương trình chính/Makefile, README và tổng hợp báo cáo |
| **TV2 – Huỳnh Long Nhật** | Cấu trúc dữ liệu AVL, phép quay, thao tác trên cây và kiểm thử AVL |
| **TV3 – Trần Văn Tài** | Mô hình mặt hàng và nghiệp vụ quản lý kho: CRUD, nhập/xuất, cảnh báo, dữ liệu mẫu |
| **TV4 – Hoàng Xuân Nhuật** | Hóa đơn, thống kê doanh thu, mặt hàng bán chạy và benchmark |
| **TV5 – Nguyễn Duy Khang** | FileIO, lưu/khôi phục dữ liệu, giao diện console, tài liệu demo |
| **Cả nhóm** | Tích hợp, kiểm thử, đọc chéo và hoàn thiện sản phẩm/báo cáo |

Nếu cần công bố tỷ lệ đóng góp, nhóm cần thống nhất và đối chiếu bằng lịch sử commit, Pull Request/review, kết quả nghiệm thu từng phần và xác nhận của các thành viên; không suy ra tỷ lệ chỉ từ số task hoặc số dòng code.

## 12. Tài liệu và minh chứng

- Báo cáo PDF: `docs/baocao/report.pdf`.
- Nguồn LaTeX: `docs/baocao/main.tex` và các file chương/phụ lục cùng thư mục.
- Slide và kịch bản demo: `docs/demo_slide/`.
- Kế hoạch công việc: `docs/KeHoach_BTL_DT05_ChiTiet.xlsx`.
- Ghi nhận kiểm chứng: `docs/verification/`.

## 13. Giới hạn và lưu ý

- `data/du_lieu_mau.csv` là dữ liệu mẫu phục vụ chạy thử, không phải dữ liệu kinh doanh thực tế.
- `kho.dat` là file nhị phân phụ thuộc cách bố trí dữ liệu/ABI của bản build; không nên xem đây là định dạng trao đổi độc lập compiler.
- Không commit dữ liệu cá nhân hoặc dữ liệu vận hành thật nếu chưa được phép.
- Kết quả benchmark chỉ có ý nghĩa khi đi kèm thông tin môi trường và cách đo.
- README mô tả ứng dụng C++ console trong repository; không khẳng định dự án hiện tại là website.
