# Các chỉnh sửa theo yêu cầu cấu trúc của giảng viên

Bản cập nhật này giữ nguyên nội dung kỹ thuật đã có, đồng thời chuẩn hóa tên chương, mục lục và thứ tự phần theo danh sách yêu cầu.

1. Trang bìa: giữ trang bìa hiện có và thông tin nhóm ĐT05.
2. Lời nói đầu: giữ lại, cập nhật mô tả benchmark cho đúng với phép đo thực tế (AVL so với vector đã sắp xếp dùng `std::lower_bound`; các quy mô 1.000, 10.000, 50.000 và 100.000).
3. Mục lục: có đủ Lời nói đầu, Chương 1–4, Kết luận, Tài liệu tham khảo và phụ lục; thêm mục Tài liệu tham khảo vào mục lục.
4. Chương 1: đổi tên thành “Nghiệp vụ và phân tích bài toán”.
5. Chương 2: giữ nội dung “Thiết kế cấu trúc dữ liệu và giải thuật”.
6. Chương 3: đổi tên thành “Cài đặt chi tiết và mã nguồn C++ minh họa”.
7. Chương 4: đổi tên thành “Đánh giá độ phức tạp và kết quả thử nghiệm”; bổ sung bảng tổng hợp độ phức tạp thao tác theo đúng cấu trúc AVL và nghiệp vụ hiện cài đặt.
8. Phần cuối: sắp xếp Kết luận, Tài liệu tham khảo, Phụ lục: Phân công công việc; phụ lục hướng dẫn build/test được giữ như phụ lục bổ sung.
9. Kết luận: không coi website là hạng mục của sản phẩm BTL; mô tả phạm vi ứng dụng C++ console hiện tại.

## Kiểm tra biên dịch

- Biên dịch bằng XeLaTeX ba lượt thông qua `build_report.sh` hoặc `build_report.bat`.
- PDF sau cập nhật: 48 trang A4.
- Đã render PDF để kiểm tra trang bìa, mục lục, tiêu đề các chương, bảng độ phức tạp, phần tài liệu tham khảo và phụ lục.
- Các cảnh báo `Underfull \hbox` còn lại là cảnh báo dàn trang trong một số ô bảng; không có lỗi biên dịch hoặc cảnh báo `Overfull \hbox` trong log cuối.

## Lưu ý

Khi cập nhật repository, chỉ cập nhật thư mục `docs/baocao/`. Không ghi đè `src/`, `include/`, `tests/` hoặc `Makefile` chỉ bằng gói báo cáo này. Để biên dịch lại trong repository, giữ cấu trúc `include/` hiện có vì một số đoạn code minh họa được lấy trực tiếp từ header trong thư mục đó.
