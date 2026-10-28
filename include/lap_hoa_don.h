#ifndef LAP_HOA_DON_H
#define LAP_HOA_DON_H

#include <vector>
#include <string>
#include "mathang.h"
#include "hoadon.h"
#include "AVLTree.h"

void lapHoaDon(AVLTree<std::string, MatHang>& kho, std::vector<HoaDon>& danhSachHoaDon);
bool kiemTraHetHang(const MatHang& mh, int soLuongMua);
void truTonTuDong(MatHang& mh, int soLuongMua);
void hienThiDanhSachHoaDon(const std::vector<HoaDon>& danhSachHoaDon);

#endif // LAP_HOA_DON_H
