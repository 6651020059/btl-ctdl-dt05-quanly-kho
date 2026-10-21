#ifndef AVLTREE_STUB_H
#define AVLTREE_STUB_H

#include <map>
#include <vector>

template <typename K, typename V>
class AVLTree {
private:
    std::map<K, V> dulieu;

public:
    bool insert(const K& key, const V& value) {
        if (dulieu.find(key) != dulieu.end()) {
            return false;
        }
        dulieu[key] = value;
        return true;
    }

    bool remove(const K& key) {
        auto it = dulieu.find(key);
        if (it == dulieu.end()) {
            return false;
        }
        dulieu.erase(it);
        return true;
    }

    V* search(const K& key) {
        auto it = dulieu.find(key);
        if (it == dulieu.end()) {
            return nullptr;
        }
        return &(it->second);
    }

    void rangeSearch(const K& lo, const K& hi, std::vector<V>& result) const {
        result.clear();
        for (auto const& cap : dulieu) {
            if (cap.first >= lo && cap.first <= hi) {
                result.push_back(cap.second);
            }
        }
    }

    std::vector<V> inorder() const {
        std::vector<V> ds;
        for (auto const& cap : dulieu) {
            ds.push_back(cap.second);
        }
        return ds;
    }

    int size() const {
        return dulieu.size();
    }

    int height() const {
        return 1;
    }

    bool empty() const {
        return dulieu.empty();
    }

    void clear() {
        dulieu.clear();
    }
};

#endif