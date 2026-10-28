# SLIDE TÓM TẮT BÀI TẬP LỚN ĐT05 (TASK T50 - TV5)

## Slide 1: Giới thiệu đề tài & Phân công nhóm
- **Đề tài ĐT05:** Hệ thống Quản lý Hàng tồn kho & Doanh thu Siêu thị sử dụng Cây nhị phân tìm kiếm tự cân bằng AVL (`AVLTree<std::string, MatHang>`).
- **Mục tiêu:** Đảm bảo thời gian tra cứu, thêm, xóa mặt hàng luôn đạt $O(\log n)$ ngay cả khi nhập mã hàng tăng dần (`MH001` -> `MH050`), hỗ trợ Range Search $O(\log n + k)$.

## Slide 2: Thiết kế Cấu trúc dữ liệu Cây AVL (TV2)
- Chiều cao cây AVL luôn bị chặn bởi $h \le 1{,}44 \log_2(n+1)$.
- 4 phép quay tái cân bằng $O(1)$: LL (Quay phải), RR (Quay trái), LR (Quay trái - phải), RL (Quay phải - trái).
- Khóa sao chép nông (`copy constructor = delete`) chống lỗi giải phóng bộ nhớ 2 lần (`double free`).

## Slide 3: Phân hệ Quản lý Kho & Cảnh báo Tồn kho (TV3)
- 4 thao tác CRUD mặt hàng $O(\log n)$, tra cứu khoảng mã $O(\log n + k)$ và khoảng giá $O(n)$.
- Nghiệp vụ nhập/xuất kho: chặn nghiêm ngặt xuất quá số lượng tồn, tự động cảnh báo mặt hàng có `soLuongTon <= tonToiThieu` và tính lượng gợi ý nhập bổ sung `2 * tonToiThieu - soLuongTon`.

## Slide 4: Phân hệ Hóa đơn, Doanh thu & Top Bán chạy (TV4)
- Lập hóa đơn bán hàng (tối đa 20 dòng chi tiết), tra cứu mặt hàng trên cây AVL và trừ tồn tự động theo thời gian thực.
- Thống kê doanh thu theo Ngày / Tháng và xếp hạng Top $N$ mặt hàng bán chạy bằng `std::map` kết hợp `std::sort`.

## Slide 5: Lưu trữ File nhị phân an toàn & Thực nghiệm Benchmark (TV4 & TV5)
- **FileIO (`kho.dat`):** Header 24 byte (`SMK1`), cơ chế ghi file tạm `.tmp` rồi đổi tên (Atomic Write), kiểm tra kích thước từng byte.
- **Kết quả Benchmark (1.000 truy vấn):**
  - 1.000 mặt hàng: AVL nhanh hơn Vector **~31,7 lần** (281 µs vs 8.907 µs).
  - 10.000 mặt hàng: AVL nhanh hơn Vector **~179,9 lần** (485 µs vs 87.260 µs).
  - 50.000 mặt hàng: AVL nhanh hơn Vector **~585,5 lần** (788 µs vs 461.403 µs).
