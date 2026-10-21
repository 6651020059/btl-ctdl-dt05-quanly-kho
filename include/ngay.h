#ifndef NGAY_H
#define NGAY_H

// Ngày/tháng/năm dùng chung cho MatHang (ngayNhap) và HoaDon (ngayLap).
// Chỉ định nghĩa ở đây, các header khác chỉ #include "ngay.h".
struct Ngay {
    int ngay;
    int thang;
    int nam;

    Ngay() : ngay(0), thang(0), nam(0) {}
    Ngay(int d, int m, int y) : ngay(d), thang(m), nam(y) {}
};

#endif // NGAY_H
