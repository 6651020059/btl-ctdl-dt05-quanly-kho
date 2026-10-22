// AVLTree.tpp - Cài đặt các hàm template khai báo trong AVLTree.h
// Lưu ý: file này được AVLTree.h #include ở cuối, KHÔNG biên dịch riêng (đuôi .tpp nên Makefile bỏ qua).
//
// Phạm vi task T17: Node, height, balance factor, thêm node (insert).
// Các phần sau thuộc task khác, chưa cài ở đây:
//   T18: rotateLeft / rotateRight và tái cân bằng khi thêm
//   T19: remove (deleteNode, getMinValueNode)
//   T20: search, inorder     T22: rangeSearch
#ifndef AVLTREE_TPP
#define AVLTREE_TPP

// ---------------------------------------------------------------- Chiều cao & hệ số cân bằng
template <typename K, typename V>
int AVLTree<K, V>::maxInt(int a, int b) const { return a > b ? a : b; }

// Chiều cao nút: nút rỗng = 0, nút lá = 1
template <typename K, typename V>
int AVLTree<K, V>::getHeightNode(AVLNode<K, V>* node) const {
    return node ? node->height : 0;
}

// Hệ số cân bằng = chiều cao con trái - chiều cao con phải
template <typename K, typename V>
int AVLTree<K, V>::getBalanceFactor(AVLNode<K, V>* node) const {
    return node ? getHeightNode(node->left) - getHeightNode(node->right) : 0;
}

// ---------------------------------------------------------------- Thêm node (đệ quy)
// success = true nếu thêm được, false nếu trùng khóa.
// Sau khi thêm, cập nhật lại height của các nút trên đường đi.
// (Tái cân bằng bằng phép quay sẽ bổ sung ở T18.)
template <typename K, typename V>
AVLNode<K, V>* AVLTree<K, V>::insertNode(AVLNode<K, V>* node, const K& key,
                                         const V& value, bool& success) {
    if (!node) {
        success = true;
        return new AVLNode<K, V>(key, value);
    }
    if (key < node->key)       node->left  = insertNode(node->left, key, value, success);
    else if (node->key < key)  node->right = insertNode(node->right, key, value, success);
    else { success = false; return node; }   // trùng khóa: không thêm

    if (success)
        node->height = 1 + maxInt(getHeightNode(node->left), getHeightNode(node->right));
    return node;
}

// ---------------------------------------------------------------- Khởi tạo / hủy
template <typename K, typename V>
AVLTree<K, V>::AVLTree() : root(nullptr), nodeCount(0) {}

template <typename K, typename V>
AVLTree<K, V>::~AVLTree() { clear(); }

template <typename K, typename V>
void AVLTree<K, V>::destroyTree(AVLNode<K, V>* node) {
    if (!node) return;
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}

template <typename K, typename V>
void AVLTree<K, V>::clear() {
    destroyTree(root);
    root = nullptr;
    nodeCount = 0;
}

// ---------------------------------------------------------------- Hàm công khai
template <typename K, typename V>
bool AVLTree<K, V>::insert(const K& key, const V& value) {
    bool success = false;
    root = insertNode(root, key, value, success);
    if (success) ++nodeCount;
    return success;
}

template <typename K, typename V>
int AVLTree<K, V>::size() const { return nodeCount; }

template <typename K, typename V>
int AVLTree<K, V>::height() const { return getHeightNode(root); }

template <typename K, typename V>
bool AVLTree<K, V>::empty() const { return root == nullptr; }

#endif // AVLTREE_TPP