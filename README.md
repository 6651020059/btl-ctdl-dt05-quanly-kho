# ĐT05 - Hệ thống quản lý hàng tồn kho và doanh thu siêu thị (cây AVL)

Bài tập lớn học phần **Cấu trúc dữ liệu và Giải thuật**. Dự án cài đặt cây tìm kiếm nhị phân tự cân bằng `AVLTree<K,V>` bằng C++11 và ứng dụng vào danh mục hàng hóa, nhập/xuất kho, bán hàng, hóa đơn, thống kê doanh thu và lưu/khôi phục dữ liệu.

**Giảng viên hướng dẫn:** ThS. Trần Thị Dung

**Repository nhóm:** https://github.com/6651020059/btl-ctdl-dt05-quanly-kho

> **Phạm vi hiện tại:** sản phẩm trong repository là ứng dụng C++ giao diện dòng lệnh (console). Dự án chưa có frontend web, HTTP API, tài khoản/phân quyền hoặc cơ sở dữ liệu tập trung. Không thể triển khai nguyên trạng bằng GitHub Pages/Vercel thành website quản lý kho; muốn có bản web cần bổ sung frontend, backend và cơ sở dữ liệu ở một giai đoạn riêng.

## 1. Cấu trúc thư mục

```text
.
├── include/                 # AVLTree, MatHang, HoaDon, FileIO và header module
├── src/                     # main, quản lý kho, hóa đơn, thống kê, benchmark
├── tests/                   # Unit test và test tích hợp/khởi động
├── data/                    # CSV dữ liệu mẫu (50 mặt hàng)
├── docs/
│   ├── baocao/              # Nguồn LaTeX, report.pdf và bằng chứng kiểm thử
│   ├── demo_slide/          # Slide và kịch bản demo
│   └── KeHoach_BTL_DT05_ChiTiet.xlsx
├── Makefile
└── README.md
```

## 2. Thành phần chính

| Thành phần | Chức năng |
|---|---|
| `include/AVLTree.h`, `AVLTree.tpp` | Cây AVL generic, insert/remove/search/rangeSearch/inorder, const search và move semantics |
| `include/mathang.h`, `include/khohang.h` | Mô hình mặt hàng và alias `KhoHang` |
| `src/quan_ly_kho.cpp` | CRUD, nhập/xuất, cảnh báo tồn tối thiểu, nạp CSV |
| `src/lap_hoa_don.cpp` | Lập hóa đơn, kiểm tra giao dịch và cập nhật tồn |
| `src/thongkedoanhthu.cpp`, `include/thongketop.h` | Thống kê ngày/tháng và Top mặt hàng |
| `include/FileIO.h` | Đọc/ghi file nhị phân `kho.dat` |
| `src/benchmark.cpp`, `include/Chuongtrinhsinhvadodulieu.h` | Benchmark AVL với vector đã sắp xếp + `lower_bound` |

## 3. Yêu cầu môi trường

- GCC hoặc trình biên dịch C++ hỗ trợ C++11.
- GNU Make.
- XeLaTeX nếu cần biên dịch báo cáo trong `docs/baocao/`.

## 4. Biên dịch và chạy ứng dụng

Chạy các lệnh từ thư mục gốc repository:

```bash
make clean
make all
./main_app
./benchmark_app
```

Chương trình chính đọc `data/du_lieu_mau.csv` theo đường dẫn tương đối khi chưa có `kho.dat`. Vì vậy cần chạy `./main_app` từ thư mục gốc repository.

## 5. Kiểm thử

```bash
make test
make asan
```

`make test` chạy các nhóm AVL, FileIO, kho, hóa đơn/doanh thu và kiểm thử luồng khởi động. `make asan` biên dịch/chạy bộ test với AddressSanitizer và UndefinedBehaviorSanitizer. Các test dùng file tạm; `make clean` không xóa file dữ liệu `kho.dat` thực của người dùng.

Kết quả lần kiểm tra lại bản ghép 5 gói CAPNHAT7 trong môi trường Linux/container:

- Build `make clean && make all`: thành công.
- AVL: 35/35.
- FileIO: 13/13 kiểm tra.
- Hóa đơn/doanh thu: 55/55.
- Kiểm thử khởi động: 12/12.
- Kiểm thử kho: các nhóm assertion đều đạt.
- `make asan`: hoàn tất không có chẩn đoán ASan/UBSan.

Đây là kết quả trên môi trường kiểm tra được ghi nhận; nhóm vẫn nên chạy lại trên máy demo/Windows trước khi nghiệm thu cuối.

## 6. Kiểm tra tự động trên GitHub

Repository có workflow `.github/workflows/cpp-ci.yml`. Khi push hoặc tạo Pull Request, GitHub Actions cài công cụ build, chạy `make clean all` và `make test` trên Ubuntu.

## 7. Biên dịch báo cáo LaTeX

Nguồn báo cáo ở `docs/baocao/`. Trên Linux/macOS có TeX Live:

```bash
cd docs/baocao
./build_report.sh
```

Trên Windows có MiKTeX/TeX Live, chạy `build_report.bat`. Hai script biên dịch XeLaTeX hai lượt, dừng nếu có lỗi và cập nhật `report.pdf`.

## 8. Phân công và trạng thái kế hoạch

Bảng phân công theo workbook:

- TV1: T01--T15.
- TV2: T16--T25.
- TV3: T26--T34.
- TV4: T35--T43.
- TV5: T44--T52.
- Cả nhóm: T53.

Workbook trong `docs/` có snapshot lịch sử ngày 31/10/2026: 29/53 task `Done`, 13 `In Progress`, 11 `To do`. Nhóm thông tin ngày 09/11/2026 rằng các công việc nội bộ đã hoàn thành sau snapshot này; tuy nhiên, bản xuất Jira/ảnh chụp board cập nhật sau 31/10 chưa được đính kèm trong gói. Vì vậy, README và báo cáo giữ riêng số liệu snapshot lịch sử, không tự khẳng định 53/53 task đã được Jira xác nhận.

**Đối chiếu tệp kế hoạch gửi riêng:** workbook `KeHoach_BTL_DT05_ChiTiet (1)(2).xlsx` nhóm gửi kèm có 53 task nhưng tất cả vẫn ở trạng thái `To do`, khác với snapshot 31/10 trong `docs/KeHoach_BTL_DT05_ChiTiet.xlsx`. Đây có thể là bản mẫu/kế hoạch chưa cập nhật; không dùng nó để chứng minh tiến độ đã hoàn thành. Hãy cập nhật hoặc xuất lại từ Jira trước khi nộp.

**Bảng Jira nhóm:** https://st-team-mdi9ncf5.atlassian.net/jira/software/projects/DT05/list (cần đăng nhập/cấp quyền để xem). Trước khi nộp, nhóm nên xuất CSV/PDF hoặc chụp board ở trạng thái mới nhất, có mã task, assignee, deadline, trạng thái và % hoàn thành.

Nhánh `main-integration`, `avl-tree`, `module-kho`, `module-doanh-thu`, `fileio-ui` được liệt kê theo kế hoạch/README; cần đối chiếu lịch sử repository để xác nhận nhánh/PR đã thực sự được tạo, review và merge. Tỷ trọng task được giao trong phụ lục chỉ là số lượng đầu việc, không phải tỷ lệ đóng góp thực tế. Muốn xác định phần trăm đóng góp cá nhân, cần kết hợp commit theo tác giả, pull request đã được review/merge và task được nghiệm thu.

## 9. Dữ liệu và an toàn

- `data/du_lieu_mau.csv` là dữ liệu mẫu, không phải dữ liệu kinh doanh thật.
- `kho.dat` là file dữ liệu nhị phân phát sinh khi chạy, không nên commit dữ liệu thực của người dùng.
- File nhị phân phụ thuộc kích thước/cách bố trí struct của bản build; không xem `kho.dat` là định dạng trao đổi di động giữa mọi compiler/ABI.

## 10. Tài liệu

- Báo cáo: `docs/baocao/report.pdf`.
- Nguồn LaTeX: `docs/baocao/main.tex` và các file chương/phụ lục cùng thư mục.
- Slide: `docs/demo_slide/T50_Slide_BaoVe_DT05.pptx`.
- Kịch bản demo: `docs/demo_slide/T51_KichBan_Demo.md`.
- Ghi chú bàn giao và các mục cần nhóm xác minh: `docs/CAPNHAT7_GHI_CHU_BAN_GIAO.md`.
- Danh sách thay đổi bản chuẩn bị nộp: `docs/CHANGES_FOR_GITHUB.md`.
- Biên bản rà soát build/test và PDF ngày 01/11: `docs/verification/VERIFICATION_2026-11-01.md`.
- Biên bản kiểm tra lại bản cập nhật ngày 09/11: `docs/verification/VERIFICATION_2026-11-09.md`.
- Bảng đối chiếu tiêu chí YC01–YC04: `docs/verification/DOI_CHIEU_YEU_CAU_GIANG_VIEN.md`.
