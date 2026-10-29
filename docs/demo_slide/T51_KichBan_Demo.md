# KỊCH BẢN DEMO BẢO VỆ ĐỒ ÁN ĐT05 (TASK T51 - TV5)

## Thứ tự trình bày và thao tác trên chương trình `main_app`

1. **Phần 1 – Khởi động & Nạp dữ liệu mẫu (TV1 & TV5 - 2 phút):**
   - Chạy `./main_app`, giới thiệu tính năng tự động nạp 50 mặt hàng mẫu từ `data/du_lieu_mau.csv` vào cây AVL (chiều cao $h = 6$).
   - Thử nhập chữ `"abc"` hoặc số `99` tại menu chính để chứng minh chương trình bắt lỗi nhập liệu an toàn, không bị treo.

2. **Phần 2 – Cấu trúc Cây AVL & Quản lý Kho hàng (TV2 & TV3 - 3 phút):**
   - Chọn **Menu 1 (Phân hệ Quản lý Kho hàng)**:
     - Chọn `1`: Hiển thị danh sách 50 mặt hàng tăng dần theo mã (duyệt `inorder` LNR).
     - Chọn `2`: Thêm mặt hàng mới `MH051` ("Nuoc ngot 7Up 330ml", giá 10.000đ, tồn 30, tồn tối thiểu 10). Thử thêm lại `MH051` để chứng minh cây AVL chặn trùng khóa.
     - Chọn `6`: Tra cứu khoảng mã từ `MH001` đến `MH005` bằng thuật toán `rangeSearch` $O(\log n + k)$.
     - Chọn `9`: Xuất kho mặt hàng `MH002` với số lượng `100` (chứng minh chặn xuất quá tồn vì chỉ còn 8), sau đó xuất `5` sản phẩm để kích hoạt cảnh báo chạm ngưỡng tồn tối thiểu.
     - Chọn `10`: Hiển thị bảng cảnh báo tồn kho tối thiểu (các mặt hàng `MH002`, `MH004`, `MH009`, `MH014`, `MH017`, `MH025`, `MH029`, `MH035`, `MH042`, `MH043`, `MH049`).

3. **Phần 3 – Lập Hóa đơn, Thống kê Doanh thu & Top Bán chạy (TV4 - 3 phút):**
   - Chọn **Menu 2 (Phân hệ Hóa đơn & Doanh thu)**:
     - Chọn `1`: Lập hóa đơn `HD001` ngày `28 10 2026`, mua `MH001` (SL: 10) và `MH005` (SL: 5). Chứng minh tồn kho trên cây AVL giảm tự động.
     - Chọn `2`: Xem lại chi tiết hóa đơn `HD001` vừa lập.
     - Chọn `3`: Thống kê doanh thu ngày `28 10 2026` và tháng `10 2026`.
     - Chọn `4`: Xem bảng xếp hạng Top 5 mặt hàng bán chạy nhất.

4. **Phần 4 – Lưu/Đọc File nhị phân & Benchmark Hiệu năng (TV4 & TV5 - 2 phút):**
   - Chọn **Menu 4**: Chạy trực tiếp Benchmark đo thời gian tìm kiếm trên 1.000, 10.000 và 50.000 mặt hàng giữa Cây AVL và Vector.
   - Chọn **Menu 3**: Lưu dữ liệu ra file nhị phân `kho.dat` (`Menu 3 -> 1`), xóa trắng RAM (`Menu 3 -> 4`), rồi khôi phục lại từ `kho.dat` (`Menu 3 -> 2`) để chứng minh toàn bộ 51 mặt hàng và hóa đơn `HD001` được phục hồi nguyên vẹn.
