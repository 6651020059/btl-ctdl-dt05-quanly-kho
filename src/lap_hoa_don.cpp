#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <limits>
#include "mathang.h"
#include "hoadon.h"
#include "AVLTree_Stub.h"   // khi TV2 xong: đổi thành "AVLTree.h"

using namespace std;

typedef AVLTree<string, MatHang> KhoHang;

// Xóa lỗi và bỏ phần dư trong bộ đệm nhập
static void xoaBoDem() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Đọc một số nguyên dương, nhập sai thì yêu cầu nhập lại
static int docSoDuong(const char* loiNhac) {
    int x;
    while (!(cin >> x) || x <= 0) {
        xoaBoDem();
        cout << loiNhac;
    }
    return x;
}

static bool hoiTiepTuc(const char* cauHoi) {
    char c;
    cout << cauHoi << " (y/n): ";
    cin >> c;
    return c == 'y' || c == 'Y';
}

// Kiểm tra kho còn đủ hàng để bán không
bool kiemTraHetHang(const MatHang& mh, int soLuongMua) {
    return mh.soLuongTon >= soLuongMua;
}

// Trừ tồn tự động sau khi bán, cảnh báo nếu hết hàng hoặc chạm mức tồn tối thiểu
void truTonTuDong(MatHang& mh, int soLuongMua) {
    mh.soLuongTon -= soLuongMua;
    if (mh.soLuongTon == 0) {
        cout << "--> [CẢNH BÁO]: Mặt hàng " << mh.maHang << " đã HẾT HÀNG!\n";
    } else if (mh.soLuongTon <= mh.tonToiThieu) {
        cout << "--> [CẢNH BÁO]: Mặt hàng " << mh.maHang
             << " đã xuống mức tồn kho tối thiểu (" << mh.tonToiThieu << ")!\n";
    }
}

void lapHoaDon(KhoHang& kho, vector<HoaDon>& danhSachHoaDon) {
    HoaDon hd;
    string s;

    cout << "\n--- LẬP HÓA ĐƠN BÁN HÀNG ---\n";
    cout << "Nhập mã hóa đơn: ";
    cin >> s;
    strncpy(hd.maHoaDon, s.c_str(), sizeof(hd.maHoaDon) - 1);

    cout << "Nhập ngày lập (Ngay Thang Nam): ";
    while (!(cin >> hd.ngayLap.ngay >> hd.ngayLap.thang >> hd.ngayLap.nam)) {
        xoaBoDem();
        cout << "Nhập lại ngày lập (Ngay Thang Nam): ";
    }

    bool tiepTuc = true;
    while (tiepTuc && hd.soLuongChiTiet < MAX_CHI_TIET) {
        cout << "Nhập mã hàng cần mua: ";
        cin >> s;

        MatHang* mh = kho.search(s);
        if (mh == nullptr) {
            cout << "[Lỗi] Mặt hàng không tồn tại trong hệ thống!\n";
            tiepTuc = hoiTiepTuc("Tiếp tục nhập mặt hàng khác?");
            continue;
        }

        cout << "-> Tên hàng: " << mh->tenHang
             << " | Tồn kho hiện tại: " << mh->soLuongTon << endl;

        cout << "Nhập số lượng mua: ";
        int soLuong = docSoDuong("Số lượng phải là số nguyên dương, nhập lại: ");

        if (!kiemTraHetHang(*mh, soLuong)) {
            cout << "[Cảnh báo] Kho không đủ hàng! (Chỉ còn: " << mh->soLuongTon << ")\n";
            tiepTuc = hoiTiepTuc("Tiếp tục nhập mặt hàng khác?");
            continue;
        }

        ChiTietHoaDon ct;
        strncpy(ct.maHang, mh->maHang, sizeof(ct.maHang) - 1);
        ct.soLuongMua = soLuong;
        ct.donGia = mh->donGia;
        hd.tongTien += (long long)ct.soLuongMua * ct.donGia;
        hd.danhSachChiTiet[hd.soLuongChiTiet++] = ct;

        truTonTuDong(*mh, soLuong);

        if (hd.soLuongChiTiet < MAX_CHI_TIET)
            tiepTuc = hoiTiepTuc("Thêm mặt hàng khác vào hóa đơn này?");
        else
            cout << "Hóa đơn đã đủ " << MAX_CHI_TIET << " dòng.\n";
    }

    if (hd.soLuongChiTiet == 0) {
        cout << "==> Hóa đơn không có mặt hàng nào, đã hủy.\n";
        return;
    }

    danhSachHoaDon.push_back(hd);
    cout << "==> Lập hóa đơn thành công! Tổng tiền thanh toán: " << hd.tongTien << " VNĐ\n";
}
