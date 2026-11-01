# Biên bản chốt giao diện (T03)

- **Ngày họp:** 19/10/2026
- **Thành viên có mặt (5/5):**
  1. **TV1 (Nhóm trưởng):** Đoàn Quốc Nhật – MSSV: 6651020059
  2. **TV2:** Huỳnh Long Nhật – MSSV: 6651020061
  3. **TV3:** Trần Văn Tài – MSSV: 6651020077
  4. **TV4:** Hoàng Xuân Nhuật – MSSV: 6651020062
  5. **TV5:** Nguyễn Duy Khang – MSSV: 6651020037

## Nội dung đã thống nhất:
1. **Cấu trúc `MatHang`:** `maHang[16]`, `tenHang[64]`, `donGia` (`long long`, đồng), `soLuongTon` (`int`), `tonToiThieu` (`int`), `ngayNhap` (`Ngay`).
2. **Khóa chính của cây AVL:** Mã mặt hàng (`std::string` chuyển từ `maHang`), không cho phép trùng khóa.
3. **Tra cứu theo khoảng mã & khoảng giá:**
   - Khoảng mã: dùng trực tiếp `rangeSearch(lo, hi, ketQua)` trên cây AVL với độ phức tạp $O(\log n + k)$.
   - Khoảng giá: phương án A — duyệt cây `inorder()` rồi lọc theo miền giá `[giaTu, giaDen]` với độ phức tạp $O(n)$.
4. **Chữ ký 9 hàm công khai của `AVLTree<K, V>`:** `insert`, `remove`, `search`, `rangeSearch`, `inorder`, `size`, `height`, `empty`, `clear`.
5. **Cấu trúc `HoaDon`:** Tối đa `MAX_CHI_TIET = 20` dòng `ChiTietHoaDon`, tự động trừ tồn kho trên cây AVL khi lập hóa đơn.
6. **Định dạng File nhị phân (`kho.dat`):** `[FileHeader 24 bytes (magic = "SMK1", version = 1, kichThuocMH, kichThuocHD, soMatHang, soHoaDon)]` + `[MatHang x soMatHang]` + `[HoaDon x soHoaDon]`, ghi an toàn qua file tạm `kho.dat.tmp`.

- **Thay đổi so với đề xuất:** Bổ sung khóa sao chép nông (`copy constructor = delete` và `operator= = delete`) trong `AVLTree.h` (Task T25) để ngăn lỗi giải phóng bộ nhớ hai lần (`double-free`).

- **Xác nhận "Đồng ý":** TV1 [x]  TV2 [x]  TV3 [x]  TV4 [x]  TV5 [x]
