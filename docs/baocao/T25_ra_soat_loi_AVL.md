# T25 – Báo cáo Rà soát lỗi & Kiểm thử Tích hợp Cây AVL (TV2 – Huỳnh Long Nhật – 6651020061)

**Trạng thái:** Đã hoàn thành (Done – Mốc M4: 28/10/2026).
Đã rà soát toàn bộ mã nguồn tích hợp giữa `AVLTree<K, V>` với các phân hệ Quản lý Kho (TV3), Hóa đơn & Doanh thu (TV4), Lưu trữ File nhị phân (TV5) và Menu điều phối (TV1).

## 1. Kết quả chạy toàn bộ bộ kiểm thử (T17 → T25)

| Bộ test | Nội dung kiểm tra | Kết quả |
|---|---|---|
| **T17** | Thêm 1.000 khóa ngẫu nhiên, kiểm tra tính chất BST | **Đạt (100%)** |
| **T18** | Bốn phép quay LL, RR, LR, RL ở cả nút gốc và cây con | **Đạt (12/12 ca)** |
| **T19** | Xóa nút lá, nút 1 con, nút 2 con, nút gốc và tái cân bằng | **Đạt (12/12 ca)** |
| **T20** | Tìm kiếm chính xác `search()` $O(\log n)$ và duyệt `inorder()` LNR | **Đạt (100%)** |
| **T22** | Truy vấn khoảng mã `rangeSearch()` $O(\log n + k)$ | **Đạt (9/9 ca)** |
| **T23** | Bộ kiểm thử tổng hợp `tests/T23_test_suite.cpp` (phép quay, xóa, const search, move semantics và range search) | **35/35 ca ĐẠT** |
| **T25** | `make asan`: AddressSanitizer + UBSan trên các nhóm test của Makefile | **Lệnh chạy thành công, không có báo cáo sanitizer** |

## 2. Xử lý triệt để rủi ro sao chép nông (Shallow Copy / Double-Free)

Trong quá trình rà soát tích hợp (Task T25), nhóm phát hiện nếu truyền `AVLTree` theo giá trị (pass-by-value), trình biên dịch sẽ sinh hàm sao chép nông chỉ chép con trỏ `root`, dẫn tới giải phóng bộ nhớ 2 lần (`heap-use-after-free`) khi hủy đối tượng.

**Giải pháp đã áp dụng chính thức trong `include/AVLTree.h`:**
```cpp
public:
    AVLTree();
    ~AVLTree();

    // Cấm sao chép nông để tránh lỗi giải phóng bộ nhớ 2 lần (Task T25)
    AVLTree(const AVLTree&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;
    AVLTree(AVLTree&& other) noexcept;
    AVLTree& operator=(AVLTree&& other) noexcept;
```
Toàn bộ các hàm trong `quan_ly_kho.cpp`, `lap_hoa_don.cpp`, `thongketop.h` và `FileIO.h` đều đã truyền `KhoHang` theo tham chiếu (`KhoHang&` hoặc `const KhoHang&`), biên dịch sạch 100% với cờ `-Wall -Wextra -O2`.

## 3. Quy ước bảo toàn bất biến khi vận hành

- Khóa chính `K` dùng `std::string` (mã mặt hàng tối đa 15 ký tự), không cho phép trùng khóa (`insert` trả về `false` nếu mã đã tồn tại).
- Hàm `search` có overload không-const trả về `V*` để cập nhật trường nghiệp vụ; overload trên cây `const` trả `const V*`, không cho phép sửa dữ liệu qua tham chiếu chỉ đọc. Các hàm nghiệp vụ hiện chỉ sửa tên, giá, tồn và ngày nhập, không thay đổi trường `maHang` dùng làm khóa. Vì `MatHang` vẫn là struct có trường công khai, quy ước này cần tiếp tục được giữ khi bổ sung chức năng mới.
