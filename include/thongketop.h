#ifndef THONGKETOP_H
#define THONGKETOP_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <limits>
#include "hoadon.h"
#include "mathang.h"
#include "AVLTree.h"

using namespace std;

typedef AVLTree<string, MatHang> KhoHang;

// Hàm tiện ích: Xóa lỗi và làm sạch bộ đệm bàn phím khi nhập sai
static void xoaBoDemTop() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Cấu trúc hỗ trợ lưu trữ thông tin thống kê bán hàng của từng mặt hàng
struct ThongKeMatHang {
    string maHang;
    int tongSoLuongBan;
    long long tongDoanhThuMatHang;
};

// Hàm so sánh giảm dần theo số lượng bán (nếu bằng nhau thì xét doanh thu)
static bool soSanhBanChay(const ThongKeMatHang& a, const ThongKeMatHang& b) {
    if (a.tongSoLuongBan != b.tongSoLuongBan) {
        return a.tongSoLuongBan > b.tongSoLuongBan;
    }
    return a.tongDoanhThuMatHang > b.tongDoanhThuMatHang;
}

// 1. Thống kê chi tiết theo từng mặt hàng và tìm Top N mặt hàng bán chạy
inline void thongKeTopMatHangBanChay(const vector<HoaDon>& danhSachHoaDon, const KhoHang& kho) {
    if (danhSachHoaDon.empty()) {
        cout << "\n[Thông báo] Hệ thống chưa có dữ liệu hóa đơn nào để thống kê!\n";
        return;
    }

    // Dùng map để cộng dồn số lượng và doanh thu theo mã hàng: <maHang, ThongKeMatHang>
    map<string, ThongKeMatHang> mapThongKe;

    for (size_t i = 0; i < danhSachHoaDon.size(); ++i) {
        const HoaDon& hd = danhSachHoaDon[i];
        for (int j = 0; j < hd.soLuongChiTiet; ++j) {
            const ChiTietHoaDon& ct = hd.danhSachChiTiet[j];
            string ma(ct.maHang);

            mapThongKe[ma].maHang = ma;
            mapThongKe[ma].tongSoLuongBan += ct.soLuongMua;
            mapThongKe[ma].tongDoanhThuMatHang += (long long)ct.soLuongMua * ct.donGia;
        }
    }

    // Chuyển dữ liệu từ map sang vector để sắp xếp
    vector<ThongKeMatHang> danhSachThongKe;
    for (map<string, ThongKeMatHang>::const_iterator it = mapThongKe.begin(); it != mapThongKe.end(); ++it) {
        danhSachThongKe.push_back(it->second);
    }

    // Sắp xếp giảm dần theo số lượng bán
    sort(danhSachThongKe.begin(), danhSachThongKe.end(), soSanhBanChay);

    cout << "\n--- THỐNG KÊ TOP MẶT HÀNG BÁN CHẠY (TASK 38) ---\n";
    cout << "Nhập số lượng Top cần xem (ví dụ: Top 3, Top 5): ";
    int n;
    while (!(cin >> n) || n <= 0) {
        if (cin.eof()) return;
        xoaBoDemTop();
        cout << "[Lỗi] Số lượng Top phải là số nguyên dương. Nhập lại: ";
    }

    int gioiHan = min(n, (int)danhSachThongKe.size());

    cout << "\n===================================================================\n";
    cout << "                       TOP " << gioiHan << " MẶT HÀNG BÁN CHẠY NHẤT                 \n";
    cout << "===================================================================\n";
    cout << "Stt | Mã Hàng       | Tên Mặt Hàng              | Đã Bán | Doanh Thu (VNĐ)\n";
    cout << "-------------------------------------------------------------------\n";

    for (int i = 0; i < gioiHan; ++i) {
        const ThongKeMatHang& tk = danhSachThongKe[i];
        
        // Tra cứu tên hàng từ cây AVL của kho
        string tenHangHienThi = "Không rõ tên";
        MatHang* mh = kho.search(tk.maHang);
        if (mh != nullptr) {
            tenHangHienThi = string(mh->tenHang);
        }

        cout << " " << (i + 1) << "   | " 
             << tk.maHang;
        
        // Căn chỉnh khoảng trắng cho đẹp giao diện console
        int khoangTrangMa = 13 - tk.maHang.length();
        for (int k = 0; k < khoangTrangMa; ++k) cout << " ";

        cout << "| " << tenHangHienThi;
        int khoangTrangTen = 25 - tenHangHienThi.length();
        for (int k = 0; k < khoangTrangTen && k >= 0; ++k) cout << " ";

        cout << "| " << tk.tongSoLuongBan;
        int khoangTrangSL = 7 - to_string(tk.tongSoLuongBan).length();
        for (int k = 0; k < khoangTrangSL; ++k) cout << " ";

        cout << "| " << tk.tongDoanhThuMatHang << endl;
    }
    cout << "===================================================================\n";
}
#endif // THONGKETOP_H
