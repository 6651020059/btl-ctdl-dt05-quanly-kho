#ifndef AVLTREE_H
#define AVLTREE_H

#include <iostream>
#include <string>
#include "Product.h"

// 1. Cấu trúc Nút trên Cây AVL
struct AVLNode {
    Product data;           // Dữ liệu sản phẩm
    int height;             // Chiều cao của nút
    AVLNode* left;          // Con trỏ cây con trái
    AVLNode* right;         // Con trỏ cây con phải

    AVLNode(const Product& p);
};

// 2. Lớp quản lý Cây cân bằng AVL (Bản khung cho nhiệm vụ DT05-16)
class AVLTree {
private:
    AVLNode* root;

    // --- Các hàm bổ trợ nội bộ ---
    int getHeight(AVLNode* node) const;
    int getBalanceFactor(AVLNode* node) const;
    int max(int a, int b) const;

    // --- Các phép quay cân bằng cây AVL (LL, RR, LR, RL) ---
    AVLNode* rotateRight(AVLNode* y);
    AVLNode* rotateLeft(AVLNode* x);

    // --- Các hàm thao tác đệ quy nội bộ ---
    AVLNode* insertNode(AVLNode* node, const Product& p);
    AVLNode* deleteNode(AVLNode* node, const std::string& maSanPham);
    AVLNode* searchNode(AVLNode* node, const std::string& maSanPham) const;
    AVLNode* getMinValueNode(AVLNode* node) const;
    void destroyTree(AVLNode* node);

    // --- Các hàm duyệt cây nội bộ ---
    void inOrderTraversal(AVLNode* node) const;
    void lowStockTraversal(AVLNode* node) const;
    void rangeSearchRevenueTraversal(AVLNode* node, double doanhThuMin, double doanhThuMax) const;

public:
    // --- Khởi tạo và Hủy ---
    AVLTree();
    ~AVLTree();

    // --- Thao tác cơ bản trên cây ---
    void insert(const Product& p);
    void remove(const std::string& maSanPham);
    AVLNode* search(const std::string& maSanPham) const;
    bool isEmpty() const;

    // --- Nghiệp vụ Siêu thị ---
    void updateStockAndRevenue(const std::string& maSanPham, int soLuongThayDoi, double donGiaXuat);
    void checkLowStock() const;                                           // Cảnh báo tồn kho tối thiểu
    void rangeSearchByRevenue(double doanhThuMin, double doanhThuMax) const; // Tra cứu phạm vi doanh thu
    void displayAllProducts() const;                                      // In danh sách sản phẩm (LNR)

    // --- Đọc / Ghi Tệp nhị phân (.dat) ---
    bool saveToFile(const std::string& tenFile) const;
    bool loadFromFile(const std::string& tenFile);
};

#endif // AVLTREE_H