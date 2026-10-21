#ifndef HOADON_H
#define HOADON_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Ngay {
    int ngay;
    int thang;
    int nam;
};

struct ChiTietHoaDon {
    char maHang[16];     
    int soLuongMua;      
    long long donGia;    
};

struct HoaDon {
    char maHoaDon[16];                 
    Ngay ngayLap;                      
    
    ChiTietHoaDon danhSachChiTiet[20]; 
    int soLuongChiTiet;                 
    long long tongTien;                  
};

#endif