#ifndef AVLTREE_H
#define AVLTREE_H

#include <vector>

// --- Cấu trúc một nút (Node) tổng quát trên cây AVL ---
template <typename K, typename V>
struct AVLNode {
    K key;               // Khóa cây AVL (Ví dụ: chuỗi mã hàng std::string hoặc char*)
    V value;             // Dữ liệu thực tế đi kèm (Ví dụ: struct MatHang)
    AVLNode* left;       // Con trỏ cây con trái
    AVLNode* right;      // Con trỏ cây con phải
    int height;          // Chiều cao nút

    AVLNode(const K& k, const V& v) 
        : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
};

// --- Lớp quản lý Cây cân bằng AVL tổng quát ---
template <typename K, typename V>
class AVLTree {
private:
    AVLNode<K, V>* root;
    int nodeCount;       // Hỗ trợ đếm nhanh kích thước cây size()

    // --- Các hàm trợ giúp đệ quy nội bộ (Nhiệm vụ cài đặt chi tiết của TV2) ---
    int getHeightNode(AVLNode<K, V>* node) const;
    int getBalanceFactor(AVLNode<K, V>* node) const;
    int max(int a, int b) const;

    AVLNode<K, V>* rotateRight(AVLNode<K, V>* y);
    AVLNode<K, V>* rotateLeft(AVLNode<K, V>* x);

    AVLNode<K, V>* insertNode(AVLNode<K, V>* node, const K& key, const V& value, bool& success);
    AVLNode<K, V>* deleteNode(AVLNode<K, V>* node, const K& key, bool& success);
    AVLNode<K, V>* getMinValueNode(AVLNode<K, V>* node) const;
    AVLNode<K, V>* searchNode(AVLNode<K, V>* node, const K& key) const;
    
    void destroyTree(AVLNode<K, V>* node);
    void inorderNode(AVLNode<K, V>* node, std::vector<V>& result) const;
    void rangeSearchNode(AVLNode<K, V>* node, const K& lo, const K& hi, std::vector<V>& result) const;

public:
    AVLTree();
    ~AVLTree();

    // --- Đầy đủ các tên hàm chính được thống nhất trong cuộc họp ---
    bool insert(const K& key, const V& value);      // Thêm phần tử, không cho trùng khóa
    bool remove(const K& key);                     // Xóa phần tử theo khóa
    V* search(const K& key) const;                 // Tìm kiếm phần tử theo khóa
    void rangeSearch(const K& lo, const K& hi, std::vector<V>& result) const; // Tra cứu khoảng mã O(log n + k)
    
    std::vector<V> inorder() const;                // Duyệt cây inorder trả về vector danh sách tăng dần
    int size() const;                              // Lấy tổng số lượng nút trên cây hiện tại
    int height() const;                            // Lấy chiều cao của toàn cây
    bool empty() const;                            // Kiểm tra cây có rỗng hay không
    void clear();                                  // Giải phóng toàn bộ cây
};

// Ép cấu trúc liên kết mã nguồn Template trong C++
#include "AVLTree.cpp"

#endif // AVLTREE_H