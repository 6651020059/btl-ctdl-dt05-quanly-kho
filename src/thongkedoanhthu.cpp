#include <iostream>
#include <limits>
#include "thongkedoanhthu.h"

using namespace std;

static void xoaBoDemThongKe() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void thongKeDoanhThuTheoNgay(const vector<HoaDon>& danhSachHoaDon) {
    Ngay ngayQuery;
    cout << "\n--- THỐNG KÊ DOANH THU THEO NGÀY ---\n";
    cout << "Nhập ngày cần thống kê (Ngay Thang Nam, ví dụ: 22 10 2026): ";
    while (!(cin >> ngayQuery.ngay >> ngayQuery.thang >> ngayQuery.nam) ||
           ngayQuery.ngay < 1 || ngayQuery.ngay > 31 ||
           ngayQuery.thang < 1 || ngayQuery.thang > 12 || ngayQuery.nam <= 0) {
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
        cout << "      MENU THỐNG KÊ DOANH THU (T37)     \n";
        cout << "========================================\n";
        cout << "1. Thống kê doanh thu theo ngày\n";
        cout << "2. Thống kê doanh thu theo tháng\n";
        cout << "0. Thoát về menu chính\n";
        cout << "========================================\n";
        cout << "Nhập lựa chọn của bạn: ";

        if (!(cin >> luaChon)) {
            if (cin.eof()) return;
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
                cout << "Thoát menu thống kê doanh thu.\n";
                break;
            default:
                cout << "[Lỗi] Lựa chọn không hợp lệ! Vui lòng chọn từ 0 đến 2.\n";
        }
    } while (luaChon != 0);
}
