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
- Lập hóa đơn bán hàng (tối đa 20 dòng chi tiết), kiểm tra tồn/tổng tiền theo giao dịch; chỉ trừ kho sau xác nhận cuối, EOF hoặc hủy xác nhận thì không ghi hóa đơn và không trừ tồn.
- Thống kê doanh thu theo Ngày / Tháng và xếp hạng Top $N$ mặt hàng bán chạy bằng `std::map` kết hợp `std::sort`.

## Slide 5: Lưu trữ File nhị phân an toàn & Thực nghiệm Benchmark (TV4 & TV5)
- **FileIO (`kho.dat`):** Header 24 byte (`SMK1`), cơ chế ghi file tạm `.tmp` rồi đổi tên (Atomic Write), kiểm tra kích thước từng byte.
- **Kết quả Benchmark (1.000 truy vấn):**
  - 1.000 mặt hàng: AVL **100,089 µs**, Vector + `lower_bound` **64,496 µs** (vector nhanh hơn khoảng **1,56 lần**).
  - 10.000 mặt hàng: AVL **192,745 µs**, Vector + `lower_bound` **122,081 µs** (vector nhanh hơn khoảng **1,58 lần**).
  - 50.000 mặt hàng: AVL **284,702 µs**, Vector + `lower_bound` **187,238 µs** (vector nhanh hơn khoảng **1,52 lần**).
  - 100.000 mặt hàng: AVL **389,176 µs**, Vector + `lower_bound` **216,581 µs** (vector nhanh hơn khoảng **1,80 lần**).
  - Ở 100.000 mặt hàng, 100 lần chèn: AVL **23,595 µs**, vector **40.817,120 µs**; 100 lần xóa: AVL **177,273 µs**, vector **21.948,953 µs**.

> Chuẩn hóa: trung vị 9 lần đo, mỗi lần 1.000 truy vấn; vector đã sắp xếp và dùng `lower_bound` (O(log n), không còn baseline duyệt tuyến tính). GCC 14.2.0 / Linux x86_64 / AMD EPYC 9V74 (3 CPU logic cấp cho container). Kết quả tương ứng `benchmark_console.txt` trong gói; thời gian thay đổi theo tải máy và môi trường.
