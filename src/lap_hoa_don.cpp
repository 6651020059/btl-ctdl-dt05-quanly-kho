#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstring>
#include <limits>
#include <map>
#include <climits>
#include "lap_hoa_don.h"
#include "nghiep_vu_ton.h"

using namespace std;


static void xoaBoDem() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

static bool kiemTraNgayHopLe(const Ngay& d) {
    return ngayHopLe(d);
}

// Đọc cả dòng để các đầu vào "yes" / "no" không để ký tự thừa trong bộ đệm.
static bool docXacNhan(const char* cauHoi) {
    string dong;
    while (true) {
        cout << cauHoi << " (y/n): ";
        if (!getline(cin >> ws, dong)) return false;
        if (dong.empty()) {
            cout << "Vui lòng nhập y/yes hoặc n/no.\n";
            continue;
        }
        char c = dong[0];
        if (c == 'y' || c == 'Y') return true;
        if (c == 'n' || c == 'N') return false;
        cout << "Vui lòng nhập y/yes hoặc n/no.\n";
    }
}

static bool docSoDuong(const char* loiNhac, int& giaTri) {
    while (true) {
        if (cin >> giaTri) {
            if (giaTri > 0) return true;
            cout << loiNhac;
        } else {
            if (cin.eof()) return false;
            xoaBoDem();
            cout << loiNhac;
        }
    }
}

bool kiemTraHetHang(const MatHang& mh, int soLuongMua) {
    return soLuongMua > 0 && mh.soLuongTon >= soLuongMua;
}

void truTonTuDong(MatHang& mh, int soLuongMua) {
    (void)giamTonKho(mh, soLuongMua);
}

void lapHoaDon(KhoHang& kho, vector<HoaDon>& danhSachHoaDon) {
    if (kho.empty()) {
        cout << "[Lỗi] Kho hàng hiện đang trống, không thể lập hóa đơn!\n";
        return;
    }

    HoaDon hd;
    string s;
    cout << "\n--- LẬP HÓA ĐƠN BÁN HÀNG ---\n";
    cout << "Nhập mã hóa đơn (tối đa 15 ký tự, vd: HD001): ";
    while (cin >> s) {
        if (!s.empty() && s.length() <= 15) {
            bool trung = false;
            for (size_t i = 0; i < danhSachHoaDon.size(); ++i) {
                if (strncmp(danhSachHoaDon[i].maHoaDon, s.c_str(), sizeof(danhSachHoaDon[i].maHoaDon)) == 0) {
                    trung = true;
                    break;
                }
            }
            if (!trung) break;
            cout << "[Lỗi] Mã hóa đơn '" << s << "' đã tồn tại. Nhập mã khác: ";
        } else {
            cout << "[Lỗi] Mã hóa đơn phải từ 1..15 ký tự. Nhập lại: ";
        }
    }
    if (cin.eof()) return;
    strncpy(hd.maHoaDon, s.c_str(), sizeof(hd.maHoaDon) - 1);

    cout << "Nhập ngày lập (Ngay Thang Nam, vd: 25 10 2026): ";
    while (!(cin >> hd.ngayLap.ngay >> hd.ngayLap.thang >> hd.ngayLap.nam) || !kiemTraNgayHopLe(hd.ngayLap)) {
        if (cin.eof()) return;
        xoaBoDem();
        cout << "[Lỗi] Ngày không hợp lệ. Nhập lại ngày lập (Ngay Thang Nam): ";
    }

    // Giai đoạn 1: chỉ ghi nhận các dòng dự kiến. Chưa thay đổi tồn kho.
    map<string, long long> soLuongChoTheoMa;
    bool tiepTuc = true;
    while (tiepTuc && hd.soLuongChiTiet < MAX_CHI_TIET) {
        cout << "Nhập mã hàng cần mua: ";
        if (!(cin >> s)) break;

        MatHang* mh = kho.search(s);
        if (mh == nullptr) {
            cout << "[Lỗi] Mặt hàng '" << s << "' không tồn tại trong hệ thống!\n";
            tiepTuc = docXacNhan("Tiếp tục nhập mặt hàng khác?");
            continue;
        }

        cout << "-> Tên hàng: " << mh->tenHang
             << " | Đơn giá: " << mh->donGia << " đ"
             << " | Tồn kho hiện tại: " << mh->soLuongTon << endl;

        if (mh->soLuongTon <= 0) {
            cout << "[Cảnh báo] Mặt hàng này đã hết hàng trong kho!\n";
            tiepTuc = docXacNhan("Tiếp tục nhập mặt hàng khác?");
            continue;
        }

        cout << "Nhập số lượng mua: ";
        int soLuong = 0;
        if (!docSoDuong("Số lượng phải là số nguyên dương, nhập lại: ", soLuong)) break;

        const long long daChon = soLuongChoTheoMa[s];
        if (daChon > mh->soLuongTon || soLuong > mh->soLuongTon - daChon) {
            cout << "[Cảnh báo] Kho không đủ hàng cho tổng số lượng đã chọn! (Yêu cầu thêm: " << soLuong
                 << ", Tồn hiện tại: " << mh->soLuongTon << ", Đã chọn: " << daChon << ")\n";
            tiepTuc = docXacNhan("Tiếp tục nhập mặt hàng khác?");
            continue;
        }

        if (mh->donGia < 0 || (mh->donGia > 0 && soLuong > LLONG_MAX / mh->donGia)) {
            cout << "[Lỗi] Thành tiền của dòng hàng vượt giới hạn kiểu long long; dòng này bị từ chối.\n";
            tiepTuc = docXacNhan("Tiếp tục nhập mặt hàng khác?");
            continue;
        }
        const long long thanhTien = static_cast<long long>(soLuong) * mh->donGia;
        if (thanhTien > LLONG_MAX - hd.tongTien) {
            cout << "[Lỗi] Tổng tiền hóa đơn vượt giới hạn kiểu long long; dòng này bị từ chối.\n";
            tiepTuc = docXacNhan("Tiếp tục nhập mặt hàng khác?");
            continue;
        }

        ChiTietHoaDon ct;
        memcpy(ct.maHang, mh->maHang, sizeof(ct.maHang));
        ct.soLuongMua = soLuong;
        ct.donGia = mh->donGia;
        hd.tongTien += thanhTien;
        hd.danhSachChiTiet[hd.soLuongChiTiet++] = ct;
        soLuongChoTheoMa[s] += soLuong;

        if (hd.soLuongChiTiet < MAX_CHI_TIET)
            tiepTuc = docXacNhan("Thêm mặt hàng khác vào hóa đơn này?");
        else
            cout << "Hóa đơn đã đạt giới hạn tối đa " << MAX_CHI_TIET << " dòng chi tiết.\n";
    }

    if (hd.soLuongChiTiet == 0) {
        cout << "==> Hóa đơn không có mặt hàng nào, đã hủy.\n";
        return;
    }

    // EOF khi đọc danh sách hoặc ở bước xác nhận đều hủy giao dịch: chưa trừ kho.
    if (cin.eof()) {
        cout << "==> Đã gặp EOF trước khi xác nhận. Hóa đơn bị hủy, tồn kho không thay đổi.\n";
        return;
    }
    if (!docXacNhan("Xác nhận lập hóa đơn và trừ kho?")) {
        cout << "==> Đã hủy hóa đơn theo yêu cầu. Tồn kho không thay đổi.\n";
        return;
    }

    // Giai đoạn 2: kiểm tra lại tồn và lưu hóa đơn trước khi trừ kho.
    for (map<string, long long>::const_iterator it = soLuongChoTheoMa.begin(); it != soLuongChoTheoMa.end(); ++it) {
        MatHang* mh = kho.search(it->first);
        if (!mh || it->second > mh->soLuongTon) {
            cout << "[Lỗi] Tồn kho đã thay đổi; hóa đơn bị hủy và không trừ kho.\n";
            return;
        }
    }
    danhSachHoaDon.push_back(hd);
    for (map<string, long long>::const_iterator it = soLuongChoTheoMa.begin(); it != soLuongChoTheoMa.end(); ++it) {
        MatHang* mh = kho.search(it->first);
        if (mh) truTonTuDong(*mh, static_cast<int>(it->second));
    }
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
