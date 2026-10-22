#ifndef AVLTREE_H
#define AVLTREE_H

#include <vector>

// --- Nút tổng quát trên cây AVL ---
template <typename K, typename V>
struct AVLNode {
    K key;               // Khóa: mã hàng (dùng std::string, không dùng char[16])
    V value;             // Dữ liệu đi kèm (vd: MatHang)
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(const K& k, const V& v)
        : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
};

// --- Cây AVL tổng quát ---
// Dùng: AVLTree<std::string, MatHang> khoHang;
template <typename K, typename V>
class AVLTree {
private:
    AVLNode<K, V>* root;
    int nodeCount;

    // Hàm trợ giúp nội bộ (TV2 cài đặt trong AVLTree.tpp)
    int getHeightNode(AVLNode<K, V>* node) const;
    int getBalanceFactor(AVLNode<K, V>* node) const;
    int maxInt(int a, int b) const;

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

    // 9 hàm chính đã thống nhất
    bool insert(const K& key, const V& value);   // không cho trùng khóa
    bool remove(const K& key);
    V* search(const K& key) const;
    void rangeSearch(const K& lo, const K& hi, std::vector<V>& result) const; // O(log n + k)

    std::vector<V> inorder() const;
    int size() const;
    int height() const;
    bool empty() const;
    void clear();
};

// Phần cài đặt template để trong .tpp (KHÔNG dùng đuôi .cpp để Makefile không biên dịch riêng)
#include "AVLTree.tpp"

#endif // AVLTREE_H
