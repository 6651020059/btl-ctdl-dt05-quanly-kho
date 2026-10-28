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

// ---------------------------------------------------------------- Thêm node (đệ quy - T17, T18)
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

// ---------------------------------------------------------------- Xóa node (T19)
template <typename K, typename V>
AVLNode<K, V>* AVLTree<K, V>::getMinValueNode(AVLNode<K, V>* node) const {
    while (node && node->left) node = node->left;
    return node;
}

template <typename K, typename V>
AVLNode<K, V>* AVLTree<K, V>::deleteNode(AVLNode<K, V>* node, const K& key, bool& success) {
    if (!node) return nullptr;

    if (key < node->key) {
        node->left = deleteNode(node->left, key, success);
    } else if (node->key < key) {
        node->right = deleteNode(node->right, key, success);
    } else {
        success = true;
        if (!node->left || !node->right) {
            AVLNode<K, V>* con = node->left ? node->left : node->right;
            delete node;
            return con;
        }
        AVLNode<K, V>* ke = getMinValueNode(node->right);
        node->key = ke->key;
        node->value = ke->value;
        bool tam = false;
        node->right = deleteNode(node->right, ke->key, tam);
    }
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

// ---------------------------------------------------------------- Tìm kiếm & duyệt (T20)
template <typename K, typename V>
AVLNode<K, V>* AVLTree<K, V>::searchNode(AVLNode<K, V>* node, const K& key) const {
    while (node) {
        if (key < node->key)       node = node->left;
        else if (node->key < key)  node = node->right;
        else                       return node;
    }
    return nullptr;
}

template <typename K, typename V>
void AVLTree<K, V>::inorderNode(AVLNode<K, V>* node, std::vector<V>& result) const {
    if (!node) return;
    inorderNode(node->left, result);
    result.push_back(node->value);
    inorderNode(node->right, result);
}

// ---------------------------------------------------------------- Range Search (T22)
template <typename K, typename V>
void AVLTree<K, V>::rangeSearchNode(AVLNode<K, V>* node, const K& lo, const K& hi,
                                    std::vector<V>& result) const {
    if (!node) return;
    if (lo < node->key) rangeSearchNode(node->left, lo, hi, result);
    if (!(node->key < lo) && !(hi < node->key)) result.push_back(node->value);
    if (node->key < hi) rangeSearchNode(node->right, lo, hi, result);
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
bool AVLTree<K, V>::remove(const K& key) {
    bool success = false;
    root = deleteNode(root, key, success);
    if (success) --nodeCount;
    return success;
}

template <typename K, typename V>
V* AVLTree<K, V>::search(const K& key) const {
    AVLNode<K, V>* n = searchNode(root, key);
    return n ? &n->value : nullptr;
}

template <typename K, typename V>
std::vector<V> AVLTree<K, V>::inorder() const {
    std::vector<V> result;
    inorderNode(root, result);
    return result;
}

template <typename K, typename V>
void AVLTree<K, V>::rangeSearch(const K& lo, const K& hi, std::vector<V>& result) const {
    if (hi < lo) return;
    rangeSearchNode(root, lo, hi, result);
}

template <typename K, typename V>
int AVLTree<K, V>::size() const { return nodeCount; }

template <typename K, typename V>
int AVLTree<K, V>::height() const { return getHeightNode(root); }

template <typename K, typename V>
bool AVLTree<K, V>::empty() const { return root == nullptr; }

#endif // AVLTREE_TPP
