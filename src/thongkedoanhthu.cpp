#include <iostream>
#include <limits>
#include "thongkedoanhthu.h"

using namespace std;

static void xoaBoDemThongKe() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Kiểm tra ngày hợp lệ theo lịch thực tế (có xét tháng 30/31 ngày và năm nhuận)
static bool ngayTruyVanHopLe(const Ngay& d) {
    return ngayHopLe(d, 1, 9999);   // lịch thực tế (ngay.h); không giới hạn 1900-2100 khi truy vấn
}

void thongKeDoanhThuTheoNgay(const vector<HoaDon>& danhSachHoaDon) {
    Ngay ngayQuery;
    cout << "\n--- THỐNG KÊ DOANH THU THEO NGÀY ---\n";
    cout << "Nhập ngày cần thống kê (Ngay Thang Nam, ví dụ: 22 10 2026): ";
    while (!(cin >> ngayQuery.ngay >> ngayQuery.thang >> ngayQuery.nam) ||
           !ngayTruyVanHopLe(ngayQuery)) {
        if (cin.eof()) return;
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

    cout << "========================================\n";
    cout << " KẾT QUẢ THỐNG KÊ NGÀY " << ngayQuery.ngay << "/" << ngayQuery.thang << "/" << ngayQuery.nam << ":\n";
    cout << " - Số lượng hóa đơn lập trong ngày: " << demHoaDon << endl;
    cout << " - Tổng doanh thu: " << tongDoanhThuNgay << " VNĐ\n";
    cout << "========================================\n";
}

void thongKeDoanhThuTheoThang(const vector<HoaDon>& danhSachHoaDon) {
    int thangQuery, namQuery;
    cout << "\n--- THỐNG KÊ DOANH THU THEO THÁNG ---\n";
    cout << "Nhập tháng và năm cần thống kê (Thang Nam, ví dụ: 10 2026): ";
    while (!(cin >> thangQuery >> namQuery) || thangQuery < 1 || thangQuery > 12 || namQuery <= 0) {
        if (cin.eof()) return;
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

    cout << "========================================\n";
    cout << " KẾT QUẢ THỐNG KÊ THÁNG " << thangQuery << "/" << namQuery << ":\n";
    cout << " - Số lượng hóa đơn lập trong tháng: " << demHoaDon << endl;
    cout << " - Tổng doanh thu: " << tongDoanhThuThang << " VNĐ\n";
    cout << "========================================\n";
}

void menuThongKeDoanhThu(const vector<HoaDon>& danhSachHoaDon) {
    int luaChon;
    do {
        cout << "\n========================================\n";
        cout << "      MENU THỐNG KÊ DOANH THU          \n";
        cout << "========================================\n";
        cout << "1. Thống kê doanh thu theo ngày\n";
        cout << "2. Thống kê doanh thu theo tháng\n";
        cout << "0. Thoát về menu chính\n";
        cout << "========================================\n";
        cout << "Nhập lựa chọn của bạn: ";

        if (!(cin >> luaChon)) {
            if (cin.eof()) return;
            xoaBoDemThongKe();
            cout << "[Lỗi] Lựa chọn phải là số 0, 1 hoặc 2. Vui lòng nhập lại.\n";
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
                cout << "Thoát menu thống kê doanh thu.\n";
                break;
            default:
                cout << "[Lỗi] Lựa chọn không hợp lệ! Vui lòng chọn từ 0 đến 2.\n";
        }
    } while (luaChon != 0);
}
