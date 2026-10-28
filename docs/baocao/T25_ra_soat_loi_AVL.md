# T25 – Rà soát lỗi cây AVL (TV2)

**Trạng thái:** task T25 là "Sửa lỗi AVL theo Jira trong đợt tích hợp" (kế hoạch 27–28/10).
Hiện **chưa có lỗi nào được báo trên Jira** cho phần AVL nên chưa có lỗi cụ thể để sửa.
Thay vào đó, TV2 đã tự rà soát trước và ghi lại kết quả bên dưới. Khi nhóm tích hợp và
có lỗi từ Jira, gửi mô tả lỗi hoặc đoạn code tái hiện thì sẽ sửa và đóng từng lỗi.

## 1. Kết quả chạy toàn bộ test (T17 → T23)

| Bộ test | Kết quả |
|---|---|
| T17 thêm 1000 khóa ngẫu nhiên | Đạt |
| T18 bốn phép quay, cân bằng sau khi thêm | Đạt |
| T19 xóa node, cân bằng sau xóa | Đạt |
| T20 tìm kiếm, inorder | Đạt |
| T22 Range Search | Đạt |
| T23 bộ test gộp (33 ca) | 33/33 đạt |
| Chạy kèm AddressSanitizer + UBSan | Không có lỗi bộ nhớ |
| Stress 1.000.000 khóa chèn tăng dần, xóa ngẫu nhiên một nửa | Đạt, chiều cao cây 20 |

## 2. Rủi ro phát hiện: sao chép cây gây lỗi bộ nhớ

`AVLTree` không khai báo hàm sao chép, nên trình biên dịch tự sinh bản sao **nông**
(chỉ chép con trỏ `root`). Nếu ai đó truyền cây theo giá trị hoặc gán cây này cho cây khác,
hai đối tượng cùng trỏ vào một cây, và khi cả hai bị hủy thì cây bị giải phóng hai lần.
Đã tái hiện bằng AddressSanitizer (`heap-use-after-free` trong `destroyTree`).

Ví dụ code gây lỗi (TV3, TV4 có thể vô tình viết):

```cpp
void inKho(AVLTree<std::string, MatHang> kho);   // truyền theo giá trị  -> LỖI
void inKho(const AVLTree<std::string, MatHang>& kho);   // truyền theo tham chiếu -> ĐÚNG
```

**Cách vá (sửa trong `AVLtree.h`, thêm hai dòng vào phần `public`):**

```cpp
public:
    AVLTree();
    ~AVLTree();
    AVLTree(const AVLTree&) = delete;              // cấm sao chép
    AVLTree& operator=(const AVLTree&) = delete;
```

Sau khi vá, code nào vô tình sao chép cây sẽ báo lỗi **ngay lúc biên dịch** thay vì
lỗi lúc chạy. Đã thử: toàn bộ test T17–T23 vẫn biên dịch và đạt với header đã vá.
Việc này cần thống nhất với cả nhóm vì `AVLtree.h` là file dùng chung (đã làm ở T16),
nên chưa tự ý sửa trên nhánh.

**Quy ước cho cả nhóm khi dùng cây:** luôn truyền `AVLTree` theo tham chiếu (`&` hoặc
`const &`), không truyền hay trả về theo giá trị.

## 3. Điều kiện sử dụng (không phải lỗi, ghi để cả nhóm biết)

- Kiểu khóa `K` phải có toán tử `<` (số, `std::string`, hoặc struct tự định nghĩa `operator<`).
  Không dùng `char*` làm khóa vì `<` so sánh địa chỉ chứ không so sánh chuỗi.
- Kiểu giá trị `V` phải gán được (`=`), vì khi xóa nút có hai con, dữ liệu của nút kế tiếp
  được chép sang nút bị xóa. `MatHang` dạng struct thông thường thỏa điều kiện này.
- Cây không cho trùng khóa: `insert` trả về `false` và giữ nguyên dữ liệu cũ. Muốn cập nhật
  mặt hàng đã có, dùng `search` lấy con trỏ rồi sửa trực tiếp.
- `search` trả về con trỏ vào nút của cây: con trỏ này **mất hiệu lực** nếu mặt hàng đó
  bị `remove` hoặc cây bị `clear`. Không giữ con trỏ lâu dài qua các lần xóa.
- `rangeSearch` **nối thêm** kết quả vào cuối `vector` truyền vào; gọi `clear()` trên
  `vector` trước nếu muốn kết quả mới hoàn toàn.
