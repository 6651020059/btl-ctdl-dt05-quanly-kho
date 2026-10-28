#ifndef QUAN_LY_KHO_H
#define QUAN_LY_KHO_H

#include <string>
#include <vector>
#include "mathang.h"
#include "AVLTree.h"

typedef AVLTree<std::string, MatHang> KhoHang;

// 1. Nhóm hàm CRUD mặt hàng cốt lõi (Task T28)
bool themMatHang(KhoHang& kho, const MatHang& mh);
bool suaMatHang(KhoHang& kho, const std::string& maHang, const char* tenMoi,
                long long donGiaMoi, int tonToiThieuMoi, Ngay ngayNhapMoi);
bool xoaMatHang(KhoHang& kho, const std::string& maHang);
MatHang* timMatHang(KhoHang& kho, const std::string& maHang);

void timTheoKhoangGia(const KhoHang& kho, long long giaTu, long long giaDen, std::vector<MatHang>& ketQua);
void timTheoKhoangMa(const KhoHang& kho, const std::string& maTu, const std::string& maDen, std::vector<MatHang>& ketQua);

// 2. Nhóm hàm nạp bộ dữ liệu mẫu >= 50 mặt hàng (Task T29)
int napDuLieuMauTuFile(KhoHang& kho, const std::string& tenFile = "data/du_lieu_mau.csv");
int khoiTaoDuLieuMau50MatHang(KhoHang& kho);

// 3. Nhóm hàm nghiệp vụ nhập kho & xuất kho (Task T30)
bool nhapKho(KhoHang& kho, const std::string& maHang, int soLuongNhap, Ngay ngayNhapMoi = Ngay(0, 0, 0));
bool xuatKho(KhoHang& kho, const std::string& maHang, int soLuongXuat);

// 4. Nhóm hàm cảnh báo tồn kho tối thiểu (Task T31)
std::vector<MatHang> layDanhSachCanhBaoTonKho(const KhoHang& kho);
void canhBaoTonKhoToiThieu(const KhoHang& kho);

// 5. Nhóm hàm hiển thị bảng & tương tác bàn phím cho Menu chính
void inTieuDeBangKho();
void inMotMatHang(const MatHang& mh);
void hienThiDanhSachKho(const KhoHang& kho);

void nhapThemMatHang(KhoHang& kho);
void nhapSuaMatHang(KhoHang& kho);
void nhapXoaMatHang(KhoHang& kho);
void nhapTimMatHang(KhoHang& kho);
void nhapNhapKho(KhoHang& kho);
void nhapXuatKho(KhoHang& kho);
void nhapTimTheoKhoangGia(const KhoHang& kho);
void nhapTimTheoKhoangMa(const KhoHang& kho);

#endif // QUAN_LY_KHO_H
