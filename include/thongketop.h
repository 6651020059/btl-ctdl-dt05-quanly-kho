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
#include "khohang.h"

// Hàm tiện ích: Xóa lỗi và làm sạch bộ đệm bàn phím khi nhập sai
inline void xoaBoDemTop() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Cấu trúc hỗ trợ lưu trữ thông tin thống kê bán hàng của từng mặt hàng
struct ThongKeMatHang {
    std::string maHang;
    int tongSoLuongBan;
    long long tongDoanhThuMatHang;
    ThongKeMatHang() : tongSoLuongBan(0), tongDoanhThuMatHang(0) {}
};

// Hàm so sánh giảm dần theo số lượng bán (nếu bằng nhau thì xét doanh thu)
inline bool soSanhBanChay(const ThongKeMatHang& a, const ThongKeMatHang& b) {
    if (a.tongSoLuongBan != b.tongSoLuongBan) {
        return a.tongSoLuongBan > b.tongSoLuongBan;
    }
    if (a.tongDoanhThuMatHang != b.tongDoanhThuMatHang)
        return a.tongDoanhThuMatHang > b.tongDoanhThuMatHang;
    return a.maHang < b.maHang;
}

// 1. Thống kê chi tiết theo từng mặt hàng và tìm Top N mặt hàng bán chạy
inline void thongKeTopMatHangBanChay(const std::vector<HoaDon>& danhSachHoaDon, const KhoHang& kho) {
    if (danhSachHoaDon.empty()) {
        std::cout << "\n[Thông báo] Hệ thống chưa có dữ liệu hóa đơn nào để thống kê!\n";
        return;
    }

    // Dùng map để cộng dồn số lượng và doanh thu theo mã hàng: <maHang, ThongKeMatHang>
    std::map<std::string, ThongKeMatHang> mapThongKe;

    for (std::size_t i = 0; i < danhSachHoaDon.size(); ++i) {
        const HoaDon& hd = danhSachHoaDon[i];
        for (int j = 0; j < hd.soLuongChiTiet; ++j) {
            const ChiTietHoaDon& ct = hd.danhSachChiTiet[j];
            std::string ma(ct.maHang);

            mapThongKe[ma].maHang = ma;
            mapThongKe[ma].tongSoLuongBan += ct.soLuongMua;
            mapThongKe[ma].tongDoanhThuMatHang += (long long)ct.soLuongMua * ct.donGia;
        }
    }

    // Chuyển dữ liệu từ map sang vector để sắp xếp
    std::vector<ThongKeMatHang> danhSachThongKe;
    for (std::map<std::string, ThongKeMatHang>::const_iterator it = mapThongKe.begin(); it != mapThongKe.end(); ++it) {
        danhSachThongKe.push_back(it->second);
    }

    // Sắp xếp giảm dần theo số lượng bán
    std::sort(danhSachThongKe.begin(), danhSachThongKe.end(), soSanhBanChay);

    std::cout << "\n--- THỐNG KÊ TOP MẶT HÀNG BÁN CHẠY ---\n";
    std::cout << "Nhập số lượng Top cần xem (ví dụ: Top 3, Top 5): ";
    int n;
    while (!(std::cin >> n) || n <= 0) {
        if (std::cin.eof()) return;
        xoaBoDemTop();
        std::cout << "[Lỗi] Số lượng Top phải là số nguyên dương. Nhập lại: ";
    }

    int gioiHan = std::min(n, (int)danhSachThongKe.size());

    std::cout << "\n===================================================================\n";
    std::cout << "                       TOP " << gioiHan << " MAT HANG BAN CHAY NHAT                 \n";
    std::cout << "===================================================================\n";
    std::cout << "STT | Ma hang       | Ten mat hang               | Da ban | Doanh thu (VND)\n";
    std::cout << "-------------------------------------------------------------------\n";

    for (int i = 0; i < gioiHan; ++i) {
        const ThongKeMatHang& tk = danhSachThongKe[i];
        
        // Tra cứu tên hàng từ cây AVL của kho
        std::string tenHangHienThi = "Không rõ tên";
        const MatHang* mh = kho.search(tk.maHang);
        if (mh != nullptr) {
            tenHangHienThi = std::string(mh->tenHang);
        }

        std::cout << " " << (i + 1) << "   | " 
             << tk.maHang;
        
        // Căn chỉnh khoảng trắng cho đẹp giao diện console
        int khoangTrangMa = 13 - tk.maHang.length();
        for (int k = 0; k < khoangTrangMa; ++k) std::cout << " ";

        std::cout << "| " << tenHangHienThi;
        int khoangTrangTen = 25 - tenHangHienThi.length();
        for (int k = 0; k < khoangTrangTen && k >= 0; ++k) std::cout << " ";

        std::cout << "| " << tk.tongSoLuongBan;
        int khoangTrangSL = 7 - std::to_string(tk.tongSoLuongBan).length();
        for (int k = 0; k < khoangTrangSL; ++k) std::cout << " ";

        std::cout << "| " << tk.tongDoanhThuMatHang << std::endl;
    }
    std::cout << "===================================================================\n";
}
#endif // THONGKETOP_H
