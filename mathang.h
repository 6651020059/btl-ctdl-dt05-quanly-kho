#ifndef PRODUCT_H
#define PRODUCT_H

#include <cstring>

struct Ngay {
    int ngay;
    int thang;
    int nam;
};

struct Product {
    char maHang[16];
    char tenHang[64];
    long long donGia;
    int soLuongTon;
    int tonToiThieu;
    Ngay ngayNhap;

    Product() {
        memset(maHang, 0, sizeof(maHang));
        memset(tenHang, 0, sizeof(tenHang));
        donGia = 0;
        soLuongTon = 0;
        tonToiThieu = 0;
        ngayNhap = {0, 0, 0};
    }

    Product(const char* ma, const char* ten, long long gia, int slTon, int tToiThieu, Ngay nn) {
        memset(maHang, 0, sizeof(maHang));
        memset(tenHang, 0, sizeof(tenHang));
        if (ma) strncpy(maHang, ma, sizeof(maHang) - 1);
        if (ten) strncpy(tenHang, ten, sizeof(tenHang) - 1);
        donGia = gia;
        soLuongTon = slTon;
        tonToiThieu = tToiThieu;
        ngayNhap = nn;
    }
};

typedef Product MatHang;

#endif