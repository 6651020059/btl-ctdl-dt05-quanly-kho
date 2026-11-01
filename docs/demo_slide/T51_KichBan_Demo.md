# KỊCH BẢN DEMO BẢO VỆ ĐỒ ÁN ĐT05 (TASK T51 – TV5: Nguyễn Duy Khang – 6651020037)

## Phân công người trình bày trực tiếp trên chương trình `./main_app`

| Thứ tự | Thời lượng | Người trình bày | Nội dung & Thao tác thực hiện trên Console |
|---|---|---|---|
| **Phần 1** | 2 phút | **TV1 – Đoàn Quốc Nhật** & **TV5 – Nguyễn Duy Khang** | Khởi động `./main_app`, giới thiệu kiến trúc tích hợp và tự động nạp 50 mặt hàng mẫu (`h = 6`). Thử nhập chuỗi `"abc"` và số `99` tại Menu để chứng minh cơ chế chống trôi lệnh và bắt lỗi nhập liệu. |
| **Phần 2** | 3 phút | **TV2 – Huỳnh Long Nhật** & **TV3 – Trần Văn Tài** | Vào **Menu 1 (Quản lý Kho)**: <br>- Chọn `1`: In danh sách 50 mặt hàng tăng dần theo mã (duyệt `inorder` LNR).<br>- Chọn `2`: Thêm `MH051` ("Nuoc ngot 7Up 330ml") và thử thêm lặp lại `MH051` để chứng minh cây AVL chặn trùng khóa.<br>- Chọn `6`: Tra cứu khoảng mã `MH001`..`MH005` bằng `rangeSearch` $O(\log n + k)$.<br>- Chọn `9` & `10`: Xuất kho `MH002` với SL `100` (chứng minh chặn xuất quá tồn), sau đó xuất `5` sản phẩm và mở Bảng cảnh báo tồn kho tối thiểu. |
| **Phần 3** | 3 phút | **TV4 – Hoàng Xuân Nhuật** | Vào **Menu 2 (Hóa đơn & Doanh thu)**:<br>- Chọn `1`: Lập hóa đơn `HD001` ngày `28 10 2026`, mua `MH001` (SL: 10) và `MH005` (SL: 5), xác nhận toàn bộ hóa đơn rồi kiểm chứng tồn kho mới giảm; thử EOF/hủy xác nhận để thấy tồn không đổi.<br>- Chọn `2`, `3`, `4`: Xem danh sách hóa đơn, thống kê doanh thu ngày/tháng và Top 5 mặt hàng bán chạy nhất. |
| **Phần 4** | 2 phút | **TV5 – Nguyễn Duy Khang** & **TV4 – Hoàng Xuân Nhuật** | - Chọn **Menu 4**: Chạy Benchmark so sánh tốc độ tìm kiếm giữa Cây AVL và Vector ở 4 quy mô (1.000 / 10.000 / 50.000 / 100.000 mặt hàng).<br>- Chọn **Menu 3**: Lưu kho ra file nhị phân `kho.dat` (`3 -> 1`), xóa trắng RAM (`3 -> 4`), sau đó nạp lại từ `kho.dat` (`3 -> 2`) để chứng minh phục hồi nguyên vẹn dữ liệu. |


## Số liệu tham chiếu đã đồng bộ với báo cáo và slide

Mỗi điểm là trung vị 9 lần đo, mỗi lần 1.000 truy vấn; tổng thời gian tính bằng microsecond.

| Quy mô | AVL (µs) | Vector sorted + lower_bound (µs) | Vector/AVL |
|---:|---:|---:|---:|
| 1.000 | 100,089 | 64,496 | 0,64 |
| 10.000 | 192,745 | 122,081 | 0,63 |
| 50.000 | 284,702 | 187,238 | 0,66 |
| 100.000 | 389,176 | 216,581 | 0,56 |

Ở mốc 100.000 mặt hàng, 100 lần chèn: AVL 23,595 µs, vector 40.817,120 µs; 100 lần xóa: AVL 177,273 µs, vector 21.948,953 µs. Số liệu là trung vị của 9 lần đo trong môi trường GCC 14.2.0 / Linux x86_64, AMD EPYC 9V74, 3 CPU logic trong container. Khi demo trên máy khác, chạy lại benchmark và nêu rõ cấu hình; không trình bày số trên như cam kết thời gian cố định.

## Checklist trước buổi bảo vệ

- [ ] Ghép 5 gói vào một repo và chạy `make clean && make -j2 && make test`.
- [ ] Chạy thử các luồng kho, bán hàng, thống kê, lưu/nạp file trên đúng máy demo.
- [ ] Chạy `./benchmark_app`; đối chiếu với số liệu trong slide nếu vẫn dùng bản build/môi trường chuẩn.
- [ ] Cả 5 thành viên tập dượt và hỏi đáp chéo. Checklist này chỉ được tick sau buổi tập dượt thực tế (T52).
