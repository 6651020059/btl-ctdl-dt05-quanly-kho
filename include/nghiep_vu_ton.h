#ifndef NGHIEP_VU_TON_H
#define NGHIEP_VU_TON_H

#include <iostream>
#include "mathang.h"

// Một điểm duy nhất thực hiện trừ tồn và phát cảnh báo ngưỡng tồn.
// Hàm không thay đổi dữ liệu nếu số lượng không hợp lệ.
inline bool giamTonKho(MatHang& mh, int soLuong) {
    if (soLuong <= 0 || soLuong > mh.soLuongTon) return false;

    mh.soLuongTon -= soLuong;
    if (mh.soLuongTon == 0) {
        std::cout << "--> [CẢNH BÁO]: Mặt hàng " << mh.maHang << " ("
                  << mh.tenHang << ") đã HẾT HÀNG!\n";
    } else if (mh.soLuongTon <= mh.tonToiThieu) {
        std::cout << "--> [CẢNH BÁO]: Mặt hàng " << mh.maHang << " ("
                  << mh.tenHang << ") đã xuống mức tồn kho tối thiểu (còn "
                  << mh.soLuongTon << " <= " << mh.tonToiThieu << ")!\n";
    }
    return true;
}

#endif // NGHIEP_VU_TON_H
