// AVLTree.h - BẢN KHUNG (chỉ khai báo), TV2 phụ trách cài đặt (T17-T20, T22).
// Tên hàm và chữ ký đã chốt ngày 19/10/2026. TV3 và TV4 dùng stub (bọc std::map)
// cùng tên hàm; khi AVL xong chỉ cần đổi #include.
#ifndef AVLTREE_H
#define AVLTREE_H

#include <functional>
#include <vector>

template <typename K, typename V>
class AVLTree {
public:
    AVLTree();
    ~AVLTree();

    bool insert(const K& key, const V& value);   // false nếu khóa đã tồn tại
    bool remove(const K& key);                   // false nếu không có khóa
    V*   search(const K& key);                   // nullptr nếu không tìm thấy

    // lấy các phần tử có lo <= key <= hi, theo thứ tự tăng dần của khóa
    void rangeSearch(const K& lo, const K& hi, std::vector<V>& ketQua) const;

    // duyệt LNR, gọi hàm visit cho từng phần tử theo thứ tự tăng dần
    void inorder(std::function<void(const K&, V&)> visit);

    int  size() const;
    int  height() const;
    bool empty() const;
    void clear();
};

#endif
