#include <iostream>
#include "stub_module.h"

using namespace std;

// TODO(TV3 - T30): nhập kho / xuất kho
void nhapXuatKho(KhoHang&) { 
    cout << "[Chưa có] Nhập/xuất kho (TV3 - T30).\n"; 
}

// TODO(TV3 - T31): cảnh báo tồn tối thiểu.
void canhBaoTonToiThieu(const KhoHang& kho) {
    cout << "[Chưa có đầy đủ] Cảnh báo tồn tối thiểu (TV3 - T31).\n";
}

// TODO(TV4 - T37, T38): thống kê doanh thu
void thongKeDoanhThu(const vector<HoaDon>& dsHD) {
    long long tong = 0;
    for (size_t i = 0; i < dsHD.size(); ++i) tong += dsHD[i].tongTien;
    cout << "[Chưa có đầy đủ] Thống kê chi tiết (TV4 - T37, T38).\n"
         << "Tạm thời: " << dsHD.size() << " hóa đơn, tổng doanh thu = " << tong << " VNĐ.\n";
}

// TODO(TV5 - T45, T46): ghi / đọc file nhị phân
void luuFile(const KhoHang&, const vector<HoaDon>&) { 
    cout << "[Chưa có] Lưu file nhị phân (TV5 - T45).\n"; 
}
void docFile(KhoHang&, vector<HoaDon>&) { 
    cout << "[Chưa có] Đọc file nhị phân (TV5 - T46).\n"; 
}

// Nạp dữ liệu mẫu để test
void napDuLieuMau(KhoHang& kho) {
    struct { const char* ma; const char* ten; long long gia; int ton; int min; } m[] = {
        {"MH001", "Gao ST25 5kg",       135000, 40, 10},
        {"MH002", "Dau an Neptune 1L",   52000, 25,  8},
        {"MH003", "Sua tuoi 1L",         32000, 60, 15},
        {"MH004", "Mi goi Hao Hao",       4500, 200, 50},
        {"MH005", "Nuoc suoi 500ml",      5000, 12, 20},
    };
    int them = 0;
    for (size_t i = 0; i < sizeof(m) / sizeof(m[0]); ++i)
        if (kho.insert(m[i].ma, MatHang(m[i].ma, m[i].ten, m[i].gia, m[i].ton, m[i].min, Ngay(15, 10, 2026)))) ++them;
    cout << "==> Đã nạp " << them << " mặt hàng mẫu.\n";
}
