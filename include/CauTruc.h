// Các cấu trúc dữ liệu dùng chung. Chốt ngày 19/10/2026 (xem README mục 6).
// Dùng mảng char kích thước cố định (không dùng std::string) để ghi nguyên cấu trúc ra file nhị phân.
#ifndef CAUTRUC_H
#define CAUTRUC_H

struct Ngay {
    int ngay, thang, nam;
};

struct MatHang {
    char maHang[16];      // khóa của cây AVL, ví dụ "SP001"
    char tenHang[64];
    long long donGia;     // đồng
    int soLuongTon;
    int tonToiThieu;      // dưới mức này thì cảnh báo
    Ngay ngayNhap;
};

struct ChiTietHoaDon {
    char maHang[16];
    int soLuong;
    long long donGia;     // giá tại thời điểm bán
};

struct HoaDon {
    int maHD;
    Ngay ngayLap;
    int soDong;                 // số dòng chi tiết đang dùng
    ChiTietHoaDon ct[20];       // tối đa 20 dòng
    long long tongTien;
};

// Header của data/mathang.bin ("MHG1") và data/hoadon.bin ("HDN1")
struct FileHeader {
    char magic[4];
    int  version;      // hiện tại là 1
    int  soBanGhi;
};

#endif
