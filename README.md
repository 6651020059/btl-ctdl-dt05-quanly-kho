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

> Cập nhật sau buổi họp chốt giao diện (19/10).

- Cấu trúc `MatHang`: (mã hàng, tên, đơn giá, số lượng tồn, tồn tối thiểu, ngày nhập)
- Khóa của cây AVL: (điền vào)
- Tên hàm chính của `AVLTree<T>`: (điền vào)
- Định dạng file nhị phân: (điền vào)

## 7. Thành viên

| TV | Họ tên | MSSV | Tài khoản GitHub | Phụ trách |
|---|---|---|---|---|
| 1 | Đoàn Quốc Nhật (nhóm trưởng) |6651020059 |6651020059| Quản lý dự án, tích hợp, ghép báo cáo |
| 2 | (điền vào) | (điền vào) | (điền vào) | Cây AVL |
| 3 | (điền vào) | (điền vào) | (điền vào) | Nghiệp vụ kho |
| 4 | (điền vào) | (điền vào) | (điền vào) | Doanh thu và đánh giá hiệu năng |
| 5 | (điền vào) | (điền vào) | (điền vào) | File, giao diện, demo |

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
