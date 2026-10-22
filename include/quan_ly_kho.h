#ifndef QUAN_LY_KHO_H
#define QUAN_LY_KHO_H

#include <string>
#include <vector>
#include "mathang.h"
#include "AVLTree_Stub.h"   // khi TV2 xong: đổi thành "AVLTree.h"

// Định nghĩa bí danh KhoHang đồng bộ với module lập hóa đơn
typedef AVLTree<std::string, MatHang> KhoHang;

// --- 4 thao tác CRUD mặt hàng cốt lõi (Task T28) ---
bool themMatHang(KhoHang& kho, const MatHang& mh);
bool suaMatHang(KhoHang& kho, const std::string& maHang, const char* tenMoi,
                long long donGiaMoi, int tonToiThieuMoi, Ngay ngayNhapMoi);
bool xoaMatHang(KhoHang& kho, const std::string& maHang);
MatHang* timMatHang(KhoHang& kho, const std::string& maHang);

// --- Tra cứu theo khoảng giá O(n) tại module kho & khoảng mã O(log n + k) ---
void timTheoKhoangGia(const KhoHang& kho, long long giaTu, long long giaDen, std::vector<MatHang>& ketQua);
void timTheoKhoangMa(const KhoHang& kho, const std::string& maTu, const std::string& maDen, std::vector<MatHang>& ketQua);

// --- Các hàm hiển thị bảng danh sách mặt hàng ---
void inTieuDeBangKho();
void inMotMatHang(const MatHang& mh);
void hienThiDanhSachKho(const KhoHang& kho);

// --- Các hàm tương tác nhập từ bàn phím (dùng cho Menu chính) ---
void nhapThemMatHang(KhoHang& kho);
void nhapSuaMatHang(KhoHang& kho);
void nhapXoaMatHang(KhoHang& kho);
void nhapTimMatHang(KhoHang& kho);

#endif // QUAN_LY_KHO_H