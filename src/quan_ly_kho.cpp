#include <iostream>
#include <iomanip>
#include <cstring>
#include <limits>
#include "quan_ly_kho.h"

using namespace std;

// Hàm phụ trợ: Xóa lỗi và bỏ phần dư trong bộ đệm bàn phím khi nhập sai
static void xoaBoDem() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Hàm phụ trợ: Kiểm tra ngày tháng năm nhập vào có hợp lệ không
static bool kiemTraNgayHopLe(const Ngay& d) {
    if (d.nam < 1900 || d.nam > 2100) return false;
    if (d.thang < 1 || d.thang > 12) return false;
    if (d.ngay < 1 || d.ngay > 31) return false;
    return true;
}

// =====================================================================
// PHẦN 1: 4 THAO TÁC CRUD CỐT LÕI TRÊN CÂY (THÊM, SỬA, XÓA, TÌM)
// =====================================================================

// 1. THÊM (Create): Thêm mặt hàng mới vào kho, khóa là maHang (không cho trùng)
bool themMatHang(KhoHang& kho, const MatHang& mh) {
    if (strlen(mh.maHang) == 0 || strlen(mh.tenHang) == 0) {
        cout << "[Lỗi] Mã hàng và tên hàng không được để trống!\n";
        return false;
    }
    if (mh.donGia < 0 || mh.soLuongTon < 0 || mh.tonToiThieu < 0) {
        cout << "[Lỗi] Đơn giá, số lượng tồn và tồn tối thiểu không được âm!\n";
        return false;
    }
    string khoa(mh.maHang);
    if (!kho.insert(khoa, mh)) {
        cout << "[Lỗi] Mã mặt hàng '" << mh.maHang << "' đã tồn tại trong kho!\n";
        return false;
    }
    return true;
}

// 2. SỬA (Update): Tìm mặt hàng theo mã và cập nhật tên, đơn giá, tồn tối thiểu, ngày nhập
bool suaMatHang(KhoHang& kho, const string& maHang, const char* tenMoi,
                long long donGiaMoi, int tonToiThieuMoi, Ngay ngayNhapMoi) {
    MatHang* mh = kho.search(maHang);
    if (mh == nullptr) {
        cout << "[Lỗi] Không tìm thấy mặt hàng có mã '" << maHang << "' để sửa!\n";
        return false;
    }
    if (donGiaMoi < 0 || tonToiThieuMoi < 0) {
        cout << "[Lỗi] Đơn giá mới và mức tồn tối thiểu mới không được âm!\n";
        return false;
    }
    if (tenMoi && strlen(tenMoi) > 0) {
        memset(mh->tenHang, 0, sizeof(mh->tenHang));
        strncpy(mh->tenHang, tenMoi, sizeof(mh->tenHang) - 1);
    }
    mh->donGia = donGiaMoi;
    mh->tonToiThieu = tonToiThieuMoi;
    if (kiemTraNgayHopLe(ngayNhapMoi)) {
        mh->ngayNhap = ngayNhapMoi;
    }
    return true;
}

// 3. XÓA (Delete): Xóa mặt hàng khỏi cây theo mã hàng
bool xoaMatHang(KhoHang& kho, const string& maHang) {
    if (!kho.remove(maHang)) {
        cout << "[Lỗi] Không tìm thấy mặt hàng có mã '" << maHang << "' để xóa!\n";
        return false;
    }
    return true;
}

// 4. TÌM KIẾM (Read): Tìm mặt hàng theo mã, trả về con trỏ tới MatHang trên cây
MatHang* timMatHang(KhoHang& kho, const string& maHang) {
    return kho.search(maHang);
}

// Tra cứu theo khoảng giá O(n): Duyệt cây inorder rồi lọc các mặt hàng nằm trong [giaTu, giaDen]
void timTheoKhoangGia(const KhoHang& kho, long long giaTu, long long giaDen, vector<MatHang>& ketQua) {
    ketQua.clear();
    vector<MatHang> ds = kho.inorder();
    for (size_t i = 0; i < ds.size(); ++i) {
        if (ds[i].donGia >= giaTu && ds[i].donGia <= giaDen) {
            ketQua.push_back(ds[i]);
        }
    }
}

// Tra cứu theo khoảng mã hàng O(log n + k): Gọi trực tiếp rangeSearch của cây AVL
void timTheoKhoangMa(const KhoHang& kho, const string& maTu, const string& maDen, vector<MatHang>& ketQua) {
    ketQua.clear();
    kho.rangeSearch(maTu, maDen, ketQua);
}

// =====================================================================
// PHẦN 2: CÁC HÀM HIỂN THỊ BẢNG VÀ NHẬP LIỆU TỪ BÀN PHÍM CHO MENU
// =====================================================================

void inTieuDeBangKho() {
    cout << string(95, '-') << "\n";
    cout << left << setw(12) << "Mã hàng"
         << setw(30) << "Tên mặt hàng"
         << right << setw(14) << "Đơn giá (đ)"
         << setw(12) << "SL Tồn"
         << setw(12) << "Tồn min"
         << "   " << left << setw(12) << "Ngày nhập" << "\n";
    cout << string(95, '-') << "\n";
}

void inMotMatHang(const MatHang& mh) {
    cout << left << setw(12) << mh.maHang
         << setw(30) << mh.tenHang
         << right << setw(14) << mh.donGia
         << setw(12) << mh.soLuongTon
         << setw(12) << mh.tonToiThieu
         << "   " << right << setfill('0') << setw(2) << mh.ngayNhap.ngay << "/"
         << setw(2) << mh.ngayNhap.thang << "/"
         << setw(4) << mh.ngayNhap.nam << setfill(' ') << "\n";
}

void hienThiDanhSachKho(const KhoHang& kho) {
    vector<MatHang> ds = kho.inorder();
    if (ds.empty()) {
        cout << "--> Kho hàng hiện đang trống!\n";
        return;
    }
    inTieuDeBangKho();
    for (size_t i = 0; i < ds.size(); ++i) {
        inMotMatHang(ds[i]);
    }
    cout << string(95, '-') << "\n";
    cout << "Tổng số mặt hàng trong kho: " << kho.size() << "\n";
}

void nhapThemMatHang(KhoHang& kho) {
    cout << "\n--- THÊM MẶT HÀNG MỚI ---\n";
    string ma, ten;
    long long gia;
    int slTon, tonMin;
    Ngay nn;

    cout << "Nhập mã hàng (tối đa 15 ký tự): ";
    cin >> ma;
    if (kho.search(ma) != nullptr) {
        cout << "[Lỗi] Mã hàng '" << ma << "' đã tồn tại trong hệ thống!\n";
        return;
    }

    xoaBoDem();
    cout << "Nhập tên mặt hàng: ";
    getline(cin, ten);

    cout << "Nhập đơn giá (VNĐ): ";
    while (!(cin >> gia) || gia < 0) {
        xoaBoDem();
        cout << "Đơn giá phải >= 0, nhập lại: ";
    }

    cout << "Nhập số lượng tồn ban đầu: ";
    while (!(cin >> slTon) || slTon < 0) {
        xoaBoDem();
        cout << "Số lượng tồn phải >= 0, nhập lại: ";
    }

    cout << "Nhập ngưỡng tồn tối thiểu: ";
    while (!(cin >> tonMin) || tonMin < 0) {
        xoaBoDem();
        cout << "Tồn tối thiểu phải >= 0, nhập lại: ";
    }

    cout << "Nhập ngày nhập (Ngay Thang Nam): ";
    while (!(cin >> nn.ngay >> nn.thang >> nn.nam) || !kiemTraNgayHopLe(nn)) {
        xoaBoDem();
        cout << "Ngày không hợp lệ, nhập lại (Ngay Thang Nam): ";
    }

    MatHang mh(ma.c_str(), ten.c_str(), gia, slTon, tonMin, nn);
    if (themMatHang(kho, mh)) {
        cout << "==> Thêm mặt hàng '" << mh.tenHang << "' thành công!\n";
    }
}

void nhapSuaMatHang(KhoHang& kho) {
    cout << "\n--- SỬA THÔNG TIN MẶT HÀNG ---\n";
    string ma, tenMoi;
    long long giaMoi;
    int tonMinMoi;
    Ngay nnMoi;

    cout << "Nhập mã mặt hàng cần sửa: ";
    cin >> ma;

    MatHang* mh = kho.search(ma);
    if (mh == nullptr) {
        cout << "[Lỗi] Không tìm thấy mặt hàng có mã '" << ma << "'!\n";
        return;
    }

    cout << "Thông tin hiện tại của mặt hàng:\n";
    inTieuDeBangKho();
    inMotMatHang(*mh);

    xoaBoDem();
    cout << "Nhập tên mới (nhấn Enter để giữ nguyên '" << mh->tenHang << "'): ";
    getline(cin, tenMoi);
    if (tenMoi.empty()) {
        tenMoi = mh->tenHang;
    }

    cout << "Nhập đơn giá mới (hiện tại " << mh->donGia << "): ";
    while (!(cin >> giaMoi) || giaMoi < 0) {
        xoaBoDem();
        cout << "Đơn giá phải >= 0, nhập lại: ";
    }

    cout << "Nhập mức tồn tối thiểu mới (hiện tại " << mh->tonToiThieu << "): ";
    while (!(cin >> tonMinMoi) || tonMinMoi < 0) {
        xoaBoDem();
        cout << "Tồn tối thiểu phải >= 0, nhập lại: ";
    }

    cout << "Nhập ngày nhập mới (Ngay Thang Nam): ";
    while (!(cin >> nnMoi.ngay >> nnMoi.thang >> nnMoi.nam) || !kiemTraNgayHopLe(nnMoi)) {
        xoaBoDem();
        cout << "Ngày không hợp lệ, nhập lại (Ngay Thang Nam): ";
    }

    if (suaMatHang(kho, ma, tenMoi.c_str(), giaMoi, tonMinMoi, nnMoi)) {
        cout << "==> Đã cập nhật mặt hàng '" << ma << "' thành công!\n";
    }
}

void nhapXoaMatHang(KhoHang& kho) {
    cout << "\n--- XÓA MẶT HÀNG ---\n";
    string ma;
    cout << "Nhập mã mặt hàng cần xóa: ";
    cin >> ma;

    MatHang* mh = kho.search(ma);
    if (mh == nullptr) {
        cout << "[Lỗi] Không tìm thấy mặt hàng có mã '" << ma << "'!\n";
        return;
    }

    cout << "-> Mặt hàng: " << mh->tenHang << " | Tồn kho hiện tại: " << mh->soLuongTon << "\n";
    char xacNhan;
    cout << "Bạn có chắc chắn muốn xóa mặt hàng này? (y/n): ";
    cin >> xacNhan;
    if (xacNhan == 'y' || xacNhan == 'Y') {
        if (xoaMatHang(kho, ma)) {
            cout << "==> Đã xóa mặt hàng '" << ma << "' khỏi kho!\n";
        }
    } else {
        cout << "==> Đã hủy thao tác xóa.\n";
    }
}

void nhapTimMatHang(KhoHang& kho) {
    cout << "\n--- TÌM KIẾM MẶT HÀNG ---\n";
    string ma;
    cout << "Nhập mã mặt hàng cần tìm: ";
    cin >> ma;

    MatHang* mh = timMatHang(kho, ma);
    if (mh == nullptr) {
        cout << "--> Không tìm thấy mặt hàng có mã '" << ma << "' trong kho!\n";
        return;
    }
    inTieuDeBangKho();
    inMotMatHang(*mh);
    cout << string(95, '-') << "\n";
}