# Biên bản rà soát kỹ thuật bản bàn giao ĐT05

Ngày kiểm tra: 01/11/2026  
Nguồn: bản ghép từ 5 gói `DT05_CAPNHAT7_TV1` đến `DT05_CAPNHAT7_TV5` được gửi trong cuộc trao đổi.  
Môi trường: Linux container, `g++ (Debian 14.2.0-19) 14.2.0`, GNU Make, XeTeX/TeX Live 2025.

## 1. Biên dịch và kiểm thử C++

| Hạng mục | Kết quả ghi nhận |
|---|---|
| `make clean && make all` | Thành công, tạo `main_app` và `benchmark_app` |
| Cây AVL | 35/35 ca đạt: 12 phép quay, 12 ca xóa, 2 ca const/move, 9 ca range search |
| FileIO | 13 kiểm tra đạt |
| Module kho | Các nhóm assertion trong `tests/test_kho.cpp` đều đạt; gồm CRUD, nhập/xuất, chặn xuất vượt tồn, chặn tràn `int`, ngày theo lịch, cảnh báo, range search và lưu/nạp |
| Hóa đơn/doanh thu | 55/55 ca đạt |
| Kiểm thử khởi động | 12/12 ca đạt |
| `make asan` | Hoàn tất; không ghi nhận chẩn đoán AddressSanitizer hoặc UndefinedBehaviorSanitizer |

Các con số trên là kết quả chạy lại trong môi trường nêu ở đầu tài liệu, không phải xác nhận rằng mọi máy demo có cùng kết quả. Nhóm nên chạy lại trên máy nộp/demo trước khi nghiệm thu.

## 2. Biên dịch báo cáo

- Lệnh: `cd docs/baocao && ./build_report.sh`.
- Script chạy XeLaTeX hai lượt với `-halt-on-error` và chép `main.pdf` thành `report.pdf`.
- Kết quả: 47 trang vật lý, khổ A4.
- Rà soát log cuối: không còn cảnh báo `Overfull \\hbox`; một số `Underfull \\hbox` ở mục lục/bố cục bảng có thể xuất hiện do cách dàn trang, nhưng không làm tràn nội dung ra ngoài lề.
- PDF đã được render thành ảnh để rà bố cục các trang, gồm bìa, mục lục, chương 1–4, kết luận và phụ lục.

## 3. Nội dung tài liệu đã đồng bộ

- Chuyển lớp báo cáo sang `extreport` với cỡ 11pt được hỗ trợ; các tệp chương/phụ lục là đầu vào bắt buộc, tránh âm thầm tạo PDF thiếu chương.
- Đồng bộ đoạn trích header hóa đơn và comparator Top N với mã nguồn hiện tại.
- Cập nhật mô tả FileIO và ma trận 13 kiểm tra theo test đang có.
- Cập nhật phụ lục phân công theo 53 task trong workbook; ghi rõ snapshot workbook 29 Done, 13 In Progress, 11 To do.
- Không khẳng định 53/53 task Jira Done vì bản xuất Jira gốc chưa có trong gói tài liệu được rà soát.
- Cập nhật số trang PDF thành 47 ở tài liệu bàn giao và các ghi chú workbook liên quan.
- README nêu rõ hiện trạng là ứng dụng C++ console; repository chưa có frontend web, HTTP API, đăng nhập/phân quyền hoặc database tập trung.

## 4. Phần nhóm cần xác minh/bổ sung trước khi nộp

1. Đối chiếu nhánh, commit và Pull Request trực tiếp trên GitHub; các tên nhánh trong phụ lục hiện là tên theo kế hoạch/README, không phải xác minh lịch sử remote.
2. Nếu muốn ghi Jira 53/53 Done, bổ sung bản xuất Jira gốc có ngày xuất, mã task, người phụ trách và trạng thái.
3. Bổ sung bằng chứng thật cho T04, T15, T52 và T53: chia sẻ báo cáo, biên nhận nộp, biên bản tập dượt và phiếu đọc chéo.
4. Chạy lại `make clean all`, `make test` và biên dịch báo cáo trên máy demo của nhóm.

## 5. Giới hạn của lần rà soát

Lần rà soát này xác minh nội dung 5 gói ZIP, các tệp LaTeX/PDF được gửi và kết quả chạy trong container. Chưa xác minh trực tiếp lịch sử GitHub, Jira, email hoặc hoạt động của các thành viên trên hệ thống ngoài những tệp đã cung cấp.
