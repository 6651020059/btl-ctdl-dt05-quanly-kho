# Biên dịch báo cáo ĐT05

## Yêu cầu

- TeX Live hoặc MiKTeX có `xelatex`.
- Các package LaTeX được dùng trong `main.tex`: `fontspec`, `babel` với Vietnamese, `amsmath`, `amssymb`, `tikz`, `pgfplots`, `graphicx`, `booktabs`, `array`, `caption`, `float`, `setspace`, `titlesec`, `hyperref`, `listings`, `xcolor`.

## Linux/macOS

Từ thư mục gốc repository:

```bash
cd docs/baocao
./build_report.sh
```

## Windows

Mở `docs/baocao/build_report.bat` hoặc chạy từ Command Prompt. Script gọi XeLaTeX ba lượt và dừng nếu có lỗi.

## Lưu ý

- `main.tex` dùng `\input` cho tất cả chương/phụ lục bắt buộc. Nếu thiếu file nguồn, biên dịch sẽ dừng thay vì tạo báo cáo có placeholder.
- Các lệnh phải chạy từ `docs/baocao` để những đường dẫn tương đối tới ảnh, log benchmark và header C++ trong các listing được phân giải đúng.
- Kết quả cuối được chép vào `docs/baocao/report.pdf`.


## Cấu trúc báo cáo theo yêu cầu giảng viên

Báo cáo được sắp xếp theo thứ tự: Trang bìa; Lời nói đầu; Mục lục; Chương 1 - Nghiệp vụ và phân tích bài toán; Chương 2 - Thiết kế cấu trúc dữ liệu và giải thuật; Chương 3 - Cài đặt chi tiết và mã nguồn C++ minh họa; Chương 4 - Đánh giá độ phức tạp và kết quả thử nghiệm; Kết luận; Tài liệu tham khảo; Phụ lục phân công công việc. Phụ lục hướng dẫn biên dịch và kiểm thử được giữ ở phần phụ lục bổ sung.


Bản PDF hiện tại được biên dịch và kiểm tra có 48 trang A4 (nằm trong giới hạn 40–50 trang). Trang bìa ghi lớp **Kỹ thuật điện tử viễn thông K66**. Khi nhóm thay đổi nội dung, cần chạy lại script và kiểm tra số trang PDF mới.

## Header C++ dùng trong code listing

Các đoạn mã minh họa trong Chương 3 lấy trích đoạn từ `../../include/`. Repository đầy đủ cần giữ các header tương ứng trong thư mục `include/`. Gói nguồn LaTeX độc lập kèm theo bốn header được dùng trực tiếp để biên dịch các listing: `lap_hoa_don.h`, `thongkedoanhthu.h`, `thongketop.h` và `Chuongtrinhsinhvadodulieu.h`. Đây là các tệp phục vụ trích mã trong báo cáo; mã nguồn C++ đầy đủ vẫn nằm trong repository dự án.
