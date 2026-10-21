#ifndef HOADON_H
#define HOADON_H

#include <cstring>
#include "ngay.h"

// HoaDon có tối đa 20 dòng ChiTietHoaDon (đã thống nhất)
const int MAX_CHI_TIET = 20;

struct ChiTietHoaDon {
    char maHang[16];
    int soLuongMua;
    long long donGia;   // đồng

    ChiTietHoaDon() : soLuongMua(0), donGia(0) {
        std::memset(maHang, 0, sizeof(maHang));
    }
};

struct HoaDon {
    char maHoaDon[16];
    Ngay ngayLap;
    ChiTietHoaDon danhSachChiTiet[MAX_CHI_TIET];
    int soLuongChiTiet;
    long long tongTien;  // đồng

    HoaDon() : ngayLap(), soLuongChiTiet(0), tongTien(0) {
        std::memset(maHoaDon, 0, sizeof(maHoaDon));
    }
};

#endif // HOADON_H
