#include <iostream>
#include <limits>
#include "thongkedoanhthu.h"

using namespace std;

// Hàm tiện ích: Xóa lỗi và làm sạch bộ đệm bàn phím khi nhập sai
static void xoaBoDemThongKe() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '
');
}

// 1. Thống kê tổng doanh thu theo một ngày cụ thể (Ngay Thang Nam)
void thongKeDoanhThuTheoNgay(const vector<HoaDon>& danhSachHoaDon) {
    Ngay ngayQuery;
    cout << "
--- THỐNG KÊ DOANH THU THEO NGÀY ---
";
    cout << "Nhập ngày cần thống kê (Ngay Thang Nam, ví dụ: 22 10 2026): ";
    while (!(cin >> ngayQuery.ngay >> ngayQuery.thang >> ngayQuery.nam)) {
        xoaBoDemThongKe();
        cout << "[Lỗi] Định dạng ngày không hợp lệ. Nhập lại (Ngay Thang Nam): ";
    }

    long long tongDoanhThuNgay = 0;
    int demHoaDon = 0;

    for (size_t i = 0; i < danhSachHoaDon.size(); ++i) {
        const HoaDon& hd = danhSachHoaDon[i];
        if (hd.ngayLap.ngay == ngayQuery.ngay &&
            hd.ngayLap.thang == ngayQuery.thang &&
            hd.ngayLap.nam == ngayQuery.nam) {
            tongDoanhThuNgay += hd.tongTien;
            demHoaDon++;
        }
    }

    cout << "========================================
";
    cout << " KẾT QUẢ THỐNG KÊ NGÀY " << ngayQuery.ngay << "/" << ngayQuery.thang << "/" << ngayQuery.nam << ":
";
    cout << " - Số lượng hóa đơn lập trong ngày: " << demHoaDon << endl;
    cout << " - Tổng doanh thu: " << tongDoanhThuNgay << " VNĐ
";
    cout << "========================================
";
}

// 2. Thống kê tổng doanh thu theo tháng và năm (Thang Nam)
void thongKeDoanhThuTheoThang(const vector<HoaDon>& danhSachHoaDon) {
    int thangQuery, namQuery;
    cout << "
--- THỐNG KÊ DOANH THU THEO THÁNG ---
";
    cout << "Nhập tháng và năm cần thống kê (Thang Nam, ví dụ: 10 2026): ";
    while (!(cin >> thangQuery >> namQuery) || thangQuery < 1 || thangQuery > 12 || namQuery <= 0) {
        xoaBoDemThongKe();
        cout << "[Lỗi] Tháng (1-12) hoặc năm không hợp lệ. Nhập lại (Thang Nam): ";
    }

    long long tongDoanhThuThang = 0;
    int demHoaDon = 0;

    for (size_t i = 0; i < danhSachHoaDon.size(); ++i) {
        const HoaDon& hd = danhSachHoaDon[i];
        if (hd.ngayLap.thang == thangQuery && hd.ngayLap.nam == namQuery) {
            tongDoanhThuThang += hd.tongTien;
            demHoaDon++;
        }
    }

    cout << "========================================
";
    cout << " KẾT QUẢ THỐNG KÊ THÁNG " << thangQuery << "/" << namQuery << ":
";
    cout << " - Số lượng hóa đơn lập trong tháng: " << demHoaDon << endl;
    cout << " - Tổng doanh thu: " << tongDoanhThuThang << " VNĐ
";
    cout << "========================================
";
}

// 3. Hàm giao diện menu lựa chọn tính năng thống kê doanh thu
void menuThongKeDoanhThu(const vector<HoaDon>& danhSachHoaDon) {
    int luaChon;
    do {
        cout << "
========================================
";
        cout << "      MENU THỐNG KÊ DOANH THU (T37)      
";
        cout << "========================================
";
        cout << "1. Thống kê doanh thu theo ngày
";
        cout << "2. Thống kê doanh thu theo tháng
";
        cout << "0. Thoát về menu chính
";
        cout << "========================================
";
        cout << "Nhập lựa chọn của bạn: ";
        
        if (!(cin >> luaChon)) {
            xoaBoDemThongKe();
            continue;
        }

        switch (luaChon) {
            case 1:
                thongKeDoanhThuTheoNgay(danhSachHoaDon);
                break;
            case 2:
                thongKeDoanhThuTheoThang(danhSachHoaDon);
                break;
            case 0:
                cout << "Thoát menu thống kê doanh thu.
";
                break;
            default:
                cout << "[Lỗi] Lựa chọn không hợp lệ! Vui lòng chọn từ 0 đến 2.
";
        }
    } while (luaChon != 0);
}
