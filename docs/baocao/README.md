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

Mở `docs/baocao/build_report.bat` hoặc chạy từ Command Prompt. Script gọi XeLaTeX hai lượt và dừng nếu có lỗi.

## Lưu ý

- `main.tex` dùng `\input` cho tất cả chương/phụ lục bắt buộc. Nếu thiếu file nguồn, biên dịch sẽ dừng thay vì tạo báo cáo có placeholder.
- Các lệnh phải chạy từ `docs/baocao` để những đường dẫn tương đối tới ảnh, log benchmark và header C++ trong các listing được phân giải đúng.
- Kết quả cuối được chép vào `docs/baocao/report.pdf`.
