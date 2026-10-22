#ifndef THONGKEDOANHTHU_H
#define THONGKEDOANHTHU_H

#include <vector>
#include "hoadon.h"
#include "ngay.h"

// Khai báo các hàm thống kê
void thongKeDoanhThuTheoNgay(const std::vector<HoaDon>& danhSachHoaDon);
void thongKeDoanhThuTheoThang(const std::vector<HoaDon>& danhSachHoaDon);
void menuThongKeDoanhThu(const std::vector<HoaDon>& danhSachHoaDon);

#endif // THONGKEDOANHTHU_H
