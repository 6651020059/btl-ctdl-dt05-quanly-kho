#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

// Cấu trúc lưu trữ thông tin sản phẩm trong siêu thị
struct Product {
    std::string maSanPham;       // Mã sản phẩm (Khóa chính trên cây AVL)
    std::string tenSanPham;      // Tên sản phẩm
    int soLuong;                 // Số lượng tồn kho hiện tại
    int nguongToiThieu;          // Ngưỡng tồn kho tối thiểu để cảnh báo
    double donGia;               // Đơn giá bán
    double doanhThu;             // Doanh thu tích lũy

    // Constructor khởi tạo sản phẩm
    Product(std::string ma = "", std::string ten = "", int sl = 0, int nguong = 0, double gia = 0.0, double dt = 0.0)
        : maSanPham(ma), tenSanPham(ten), soLuong(sl), nguongToiThieu(nguong), donGia(gia), doanhThu(dt) {}
};

#endif // PRODUCT_H