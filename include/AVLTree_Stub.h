#ifndef AVLTREE_H          // cùng guard với AVLTree.h thật: chỉ một bản được nạp
#define AVLTREE_H

// BẢN TẠM (stub) dùng std::map, cùng giao diện với AVLTree thật.
// Khi TV2 xong AVLTree.h thật: đổi #include "AVLTree_Stub.h" -> #include "AVLTree.h"
// và xóa file này.

#include <map>
#include <vector>

template <typename K, typename V>
class AVLTree {
private:
    mutable std::map<K, V> dulieu;   // mutable để search() const trả về V* như bản thật

public:
    bool insert(const K& key, const V& value) {
        return dulieu.insert(std::make_pair(key, value)).second;  // không cho trùng khóa
    }

    bool remove(const K& key) {
        return dulieu.erase(key) > 0;
    }

    V* search(const K& key) const {
        typename std::map<K, V>::iterator it = dulieu.find(key);
        return (it == dulieu.end()) ? nullptr : &(it->second);
    }

    // Tra cứu theo khoảng mã [lo, hi]
    void rangeSearch(const K& lo, const K& hi, std::vector<V>& result) const {
        result.clear();
        for (typename std::map<K, V>::const_iterator it = dulieu.lower_bound(lo);
             it != dulieu.end() && !(hi < it->first); ++it) {
            result.push_back(it->second);
        }
    }

    std::vector<V> inorder() const {
        std::vector<V> ds;
        for (typename std::map<K, V>::const_iterator it = dulieu.begin();
             it != dulieu.end(); ++it) {
            ds.push_back(it->second);
        }
        return ds;
    }

    int size() const { return static_cast<int>(dulieu.size()); }
    int height() const { return dulieu.empty() ? 0 : 1; }   // stub: không có chiều cao thật
    bool empty() const { return dulieu.empty(); }
    void clear() { dulieu.clear(); }
};

#endif // AVLTREE_H
