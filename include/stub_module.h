#ifndef STUB_MODULE_H
#define STUB_MODULE_H

#include <vector>
#include "AVLTree_Stub.h" // Tạm thời dùng stub của AVL Tree
#include "mathang.h"
#include "hoadon.h"

// Định nghĩa KhoHang tạm thời
typedef AVLTree<std::string, MatHang> KhoHang;

// Các hàm stub (tạm) thay thế cho các module chưa hoàn thành
void nhapXuatKho(KhoHang& kho);
void canhBaoTonToiThieu(const KhoHang& kho);
void thongKeDoanhThu(const std::vector<HoaDon>& dsHD);
void luuFile(const KhoHang& kho, const std::vector<HoaDon>& dsHD);
void docFile(KhoHang& kho, std::vector<HoaDon>& dsHD);
void napDuLieuMau(KhoHang& kho);

#endif // STUB_MODULE_H
