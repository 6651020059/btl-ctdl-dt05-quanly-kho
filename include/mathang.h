#ifndef MATHANG_H
#define MATHANG_H

#include <cstring>
#include "ngay.h"

// Giao diện đã thống nhất (họp 19/10/2026):
// maHang[16], tenHang[64], donGia (long long, đồng), soLuongTon, tonToiThieu, ngayNhap (Ngay)
// Khóa của cây AVL = maHang, không cho trùng.
struct MatHang {
    char maHang[16];
    char tenHang[64];
    long long donGia;
    int soLuongTon;
    int tonToiThieu;
    Ngay ngayNhap;

    MatHang() : donGia(0), soLuongTon(0), tonToiThieu(0), ngayNhap() {
        std::memset(maHang, 0, sizeof(maHang));
        std::memset(tenHang, 0, sizeof(tenHang));
    }

    MatHang(const char* ma, const char* ten, long long gia,
            int slTon, int tToiThieu, Ngay nn)
        : donGia(gia), soLuongTon(slTon), tonToiThieu(tToiThieu), ngayNhap(nn) {
        std::memset(maHang, 0, sizeof(maHang));
        std::memset(tenHang, 0, sizeof(tenHang));
        if (ma)  std::strncpy(maHang, ma, sizeof(maHang) - 1);
        if (ten) std::strncpy(tenHang, ten, sizeof(tenHang) - 1);
    }
};

// Giữ tương thích với code cũ dùng tên Product
typedef MatHang Product;

#endif // MATHANG_H
