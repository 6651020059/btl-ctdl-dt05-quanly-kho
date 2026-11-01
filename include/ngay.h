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

// ---------------------------------------------------------------------
// Kiểm tra ngày theo LỊCH THỰC TẾ - dùng chung cho mọi phân hệ
// (nhập kho, lập hóa đơn, thống kê doanh thu) để không còn 3 bản sao
// rời rạc. Ví dụ: 31/02/2026, 31/04/2026, 29/02/2027 là KHÔNG hợp lệ;
// 29/02/2028 là hợp lệ (năm nhuận).
// ---------------------------------------------------------------------

// Năm nhuận: chia hết cho 400, hoặc chia hết cho 4 mà không chia hết cho 100.
inline bool laNamNhuan(int nam) {
    return (nam % 400 == 0) || (nam % 4 == 0 && nam % 100 != 0);
}

// Số ngày của tháng `thang` trong năm `nam`; trả về 0 nếu tháng không hợp lệ.
inline int soNgayCuaThang(int thang, int nam) {
    static const int soNgay[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (thang < 1 || thang > 12) return 0;
    if (thang == 2 && laNamNhuan(nam)) return 29;
    return soNgay[thang - 1];
}

// Ngày hợp lệ khi năm nằm trong [namToiThieu, namToiDa], tháng 1..12 và
// ngày 1..số ngày thực của tháng đó.
inline bool ngayHopLe(const Ngay& d, int namToiThieu = 1900, int namToiDa = 2100) {
    if (d.nam < namToiThieu || d.nam > namToiDa) return false;
    int toiDa = soNgayCuaThang(d.thang, d.nam);
    return toiDa > 0 && d.ngay >= 1 && d.ngay <= toiDa;
}

#endif // NGAY_H
