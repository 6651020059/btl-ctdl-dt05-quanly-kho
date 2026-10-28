#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstring>
#include <limits>
#include "lap_hoa_don.h"

using namespace std;

typedef AVLTree<string, MatHang> KhoHang;

static void xoaBoDem() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

static bool kiemTraNgayHopLe(const Ngay& d) {
    if (d.nam < 1900 || d.nam > 2100) return false;
    if (d.thang < 1 || d.thang > 12) return false;
    if (d.ngay < 1 || d.ngay > 31) return false;
    return true;
}

static int docSoDuong(const char* loiNhac) {
    int x;
    while (!(cin >> x) || x <= 0) {
        if (cin.eof()) return 1;
        xoaBoDem();
        cout << loiNhac;
    }
    return x;
}

static bool hoiTiepTuc(const char* cauHoi) {
    char c;
    cout << cauHoi << " (y/n): ";
    if (!(cin >> c)) return false;
    return c == 'y' || c == 'Y';
}

bool kiemTraHetHang(const MatHang& mh, int soLuongMua) {
    return mh.soLuongTon >= soLuongMua;
}

void truTonTuDong(MatHang& mh, int soLuongMua) {
    mh.soLuongTon -= soLuongMua;
    if (mh.soLuongTon == 0) {
        cout << "--> [CẢNH BÁO]: Mặt hàng " << mh.maHang << " (" << mh.tenHang << ") đã HẾT HÀNG!\n";
    } else if (mh.soLuongTon <= mh.tonToiThieu) {
        cout << "--> [CẢNH BÁO]: Mặt hàng " << mh.maHang << " (" << mh.tenHang
             << ") đã xuống mức tồn kho tối thiểu (còn " << mh.soLuongTon
             << " <= " << mh.tonToiThieu << ")!\n";
    }
}

void lapHoaDon(KhoHang& kho, vector<HoaDon>& danhSachHoaDon) {
    if (kho.empty()) {
        cout << "[Lỗi] Kho hàng hiện đang trống, không thể lập hóa đơn!\n";
        return;
    }

    HoaDon hd;
    string s;

    cout << "\n--- LẬP HÓA ĐƠN BÁN HÀNG (T36) ---\n";
    cout << "Nhập mã hóa đơn (tối đa 15 ký tự, vd: HD001): ";
    while (cin >> s) {
        if (!s.empty() && s.length() <= 15) break;
        cout << "[Lỗi] Mã hóa đơn phải từ 1..15 ký tự. Nhập lại: ";
    }
    if (cin.eof()) return;
    strncpy(hd.maHoaDon, s.c_str(), sizeof(hd.maHoaDon) - 1);

    cout << "Nhập ngày lập (Ngay Thang Nam, vd: 25 10 2026): ";
    while (!(cin >> hd.ngayLap.ngay >> hd.ngayLap.thang >> hd.ngayLap.nam) || !kiemTraNgayHopLe(hd.ngayLap)) {
        if (cin.eof()) return;
        xoaBoDem();
        cout << "[Lỗi] Ngày không hợp lệ. Nhập lại ngày lập (Ngay Thang Nam): ";
    }

    bool tiepTuc = true;
    while (tiepTuc && hd.soLuongChiTiet < MAX_CHI_TIET) {
        cout << "Nhập mã hàng cần mua: ";
        if (!(cin >> s)) break;

        MatHang* mh = kho.search(s);
        if (mh == nullptr) {
            cout << "[Lỗi] Mặt hàng '" << s << "' không tồn tại trong hệ thống!\n";
            tiepTuc = hoiTiepTuc("Tiếp tục nhập mặt hàng khác?");
            continue;
        }

        cout << "-> Tên hàng: " << mh->tenHang
             << " | Đơn giá: " << mh->donGia << " đ"
             << " | Tồn kho hiện tại: " << mh->soLuongTon << endl;

        if (mh->soLuongTon <= 0) {
            cout << "[Cảnh báo] Mặt hàng này đã hết hàng trong kho!\n";
            tiepTuc = hoiTiepTuc("Tiếp tục nhập mặt hàng khác?");
            continue;
        }

        cout << "Nhập số lượng mua: ";
        int soLuong = docSoDuong("Số lượng phải là số nguyên dương, nhập lại: ");

        if (!kiemTraHetHang(*mh, soLuong)) {
            cout << "[Cảnh báo] Kho không đủ hàng! (Yêu cầu: " << soLuong
                 << ", Tồn hiện tại: " << mh->soLuongTon << ")\n";
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
            cout << "Hóa đơn đã đạt giới hạn tối đa " << MAX_CHI_TIET << " dòng chi tiết.\n";
    }

    if (hd.soLuongChiTiet == 0) {
        cout << "==> Hóa đơn không có mặt hàng nào, đã hủy.\n";
        return;
    }

    danhSachHoaDon.push_back(hd);
    cout << "==> Lập hóa đơn '" << hd.maHoaDon << "' thành công! Tổng tiền thanh toán: " << hd.tongTien << " VNĐ\n";
}

void hienThiDanhSachHoaDon(const vector<HoaDon>& danhSachHoaDon) {
    cout << "\n--- DANH SÁCH HÓA ĐƠN ĐÃ LẬP ---\n";
    if (danhSachHoaDon.empty()) {
        cout << "--> Chưa có hóa đơn nào trong hệ thống.\n";
        return;
    }
    cout << string(78, '-') << "\n";
    cout << left << setw(12) << "Mã HĐ"
         << setw(16) << "Ngày lập"
         << right << setw(14) << "Số dòng CT"
         << setw(22) << "Tổng tiền (VNĐ)" << "\n";
    cout << string(78, '-') << "\n";
    for (size_t i = 0; i < danhSachHoaDon.size(); ++i) {
        const HoaDon& hd = danhSachHoaDon[i];
        cout << left << setw(12) << hd.maHoaDon
             << right << setfill('0') << setw(2) << hd.ngayLap.ngay << "/"
             << setw(2) << hd.ngayLap.thang << "/"
             << setw(4) << hd.ngayLap.nam << setfill(' ') << "      "
             << right << setw(14) << hd.soLuongChiTiet
             << setw(22) << hd.tongTien << "\n";
        for (int j = 0; j < hd.soLuongChiTiet; ++j) {
            const ChiTietHoaDon& ct = hd.danhSachChiTiet[j];
            cout << "    + Mã SP: " << left << setw(10) << ct.maHang
                 << " | SL: " << right << setw(5) << ct.soLuongMua
                 << " | Đơn giá: " << setw(10) << ct.donGia
                 << " | Thành tiền: " << setw(12) << (long long)ct.soLuongMua * ct.donGia << " đ\n";
        }
    }
    cout << string(78, '-') << "\n";
    cout << "Tổng số hóa đơn: " << danhSachHoaDon.size() << "\n";
}
