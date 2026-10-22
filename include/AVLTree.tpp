#ifndef AVLTREE_TPP
#define AVLTREE_TPP

// ---------------------------------------------------------------- Chiều cao & hệ số cân bằng
template <typename K, typename V>
int AVLTree<K, V>::maxInt(int a, int b) const { return a > b ? a : b; }

template <typename K, typename V>
int AVLTree<K, V>::getHeightNode(AVLNode<K, V>* node) const {
    return node ? node->height : 0;
}

template <typename K, typename V>
int AVLTree<K, V>::getBalanceFactor(AVLNode<K, V>* node) const {
    return node ? getHeightNode(node->left) - getHeightNode(node->right) : 0;
}

// ---------------------------------------------------------------- Phép quay (T18)
template <typename K, typename V>
AVLNode<K, V>* AVLTree<K, V>::rotateRight(AVLNode<K, V>* y) {
    AVLNode<K, V>* x = y->left;
    y->left = x->right;
    x->right = y;
    
    y->height = 1 + maxInt(getHeightNode(y->left), getHeightNode(y->right));
    x->height = 1 + maxInt(getHeightNode(x->left), getHeightNode(x->right));
    return x;
}

template <typename K, typename V>
AVLNode<K, V>* AVLTree<K, V>::rotateLeft(AVLNode<K, V>* x) {
    AVLNode<K, V>* y = x->right;
    x->right = y->left;
    y->left = x;
    
    x->height = 1 + maxInt(getHeightNode(x->left), getHeightNode(x->right));
    y->height = 1 + maxInt(getHeightNode(y->left), getHeightNode(y->right));
    return y;
}

// ---------------------------------------------------------------- Thêm node (đệ quy)
template <typename K, typename V>
AVLNode<K, V>* AVLTree<K, V>::insertNode(AVLNode<K, V>* node, const K& key,
                                         const V& value, bool& success) {
    if (!node) {
        success = true;
        return new AVLNode<K, V>(key, value);
    }
    if (key < node->key)       node->left  = insertNode(node->left, key, value, success);
    else if (node->key < key)  node->right = insertNode(node->right, key, value, success);
    else { success = false; return node; }

    if (!success) return node;

    node->height = 1 + maxInt(getHeightNode(node->left), getHeightNode(node->right));
    int bf = getBalanceFactor(node);

    if (bf > 1) {
        if (getBalanceFactor(node->left) < 0)
            node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (bf < -1) {
        if (getBalanceFactor(node->right) > 0)
            node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
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
