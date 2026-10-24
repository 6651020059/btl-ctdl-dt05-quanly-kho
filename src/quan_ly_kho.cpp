#include <iostream>
#include <iomanip>
#include <cstring>
#include <limits>
#include <fstream>
#include <sstream>
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
// PHẦN 1: 4 THAO TÁC CRUD CỐT LÕI TRÊN CÂY (TASK T28)
// =====================================================================

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

bool xoaMatHang(KhoHang& kho, const string& maHang) {
    if (!kho.remove(maHang)) {
        cout << "[Lỗi] Không tìm thấy mặt hàng có mã '" << maHang << "' để xóa!\n";
        return false;
    }
    return true;
}

MatHang* timMatHang(KhoHang& kho, const string& maHang) {
    return kho.search(maHang);
}

void timTheoKhoangGia(const KhoHang& kho, long long giaTu, long long giaDen, vector<MatHang>& ketQua) {
    ketQua.clear();
    if (giaTu > giaDen) {
        long long tam = giaTu;
        giaTu = giaDen;
        giaDen = tam;
    }
    vector<MatHang> ds = kho.inorder();
    for (size_t i = 0; i < ds.size(); ++i) {
        if (ds[i].donGia >= giaTu && ds[i].donGia <= giaDen) {
            ketQua.push_back(ds[i]);
        }
    }
}

void timTheoKhoangMa(const KhoHang& kho, const string& maTu, const string& maDen, vector<MatHang>& ketQua) {
    ketQua.clear();
    if (maTu <= maDen) {
        kho.rangeSearch(maTu, maDen, ketQua);
    } else {
        kho.rangeSearch(maDen, maTu, ketQua);
    }
}

// =====================================================================
// PHẦN 2: BỘ DỮ LIỆU MẪU >= 50 MẶT HÀNG (TASK T29)
// =====================================================================

int khoiTaoDuLieuMau50MatHang(KhoHang& kho) {
    const MatHang dsMau[50] = {
        MatHang("MH001", "Sua tuoi Vinamilk 100% 1L", 32000, 50, 10, Ngay(1, 10, 2026)),
        MatHang("MH002", "Thung mi Hao Hao tom chua cay", 115000, 8, 15, Ngay(2, 10, 2026)),
        MatHang("MH003", "Dau an Neptune Light 1L", 52000, 25, 10, Ngay(3, 10, 2026)),
        MatHang("MH004", "Nuoc mam Nam Ngu 750ml", 42000, 6, 12, Ngay(4, 10, 2026)),
        MatHang("MH005", "Gao thom ST25 tui 5kg", 185000, 40, 10, Ngay(5, 10, 2026)),
        MatHang("MH006", "Duong tinh luyen Bien Hoa 1kg", 26000, 35, 15, Ngay(5, 10, 2026)),
        MatHang("MH007", "Muoi I-ot Hai Chau goi 500g", 7000, 60, 20, Ngay(6, 10, 2026)),
        MatHang("MH008", "Bot ngot Ajinomoto goi 1kg", 65000, 28, 10, Ngay(6, 10, 2026)),
        MatHang("MH009", "Hat nem Knorr thit than 900g", 78000, 9, 10, Ngay(7, 10, 2026)),
        MatHang("MH010", "Tuong ot Chin-su chai 250g", 16000, 45, 15, Ngay(7, 10, 2026)),
        MatHang("MH011", "Tuong ca Cholimex chai 270g", 15000, 30, 10, Ngay(8, 10, 2026)),
        MatHang("MH012", "Nuoc tuong Maggi dau nanh 700ml", 24000, 40, 15, Ngay(8, 10, 2026)),
        MatHang("MH013", "Sua dac Ong Tho trang 380g", 25000, 55, 20, Ngay(9, 10, 2026)),
        MatHang("MH014", "Ca phe hoa tan G7 3in1 hop 21 goi", 58000, 7, 10, Ngay(9, 10, 2026)),
        MatHang("MH015", "Tra tui loc Lipton Nhan Vang", 43000, 18, 8, Ngay(10, 10, 2026)),
        MatHang("MH016", "Thung 24 lon Coca Cola 320ml", 210000, 12, 8, Ngay(10, 10, 2026)),
        MatHang("MH017", "Thung 24 lon Pepsi 320ml", 205000, 4, 8, Ngay(11, 10, 2026)),
        MatHang("MH018", "Thung 24 lon Bia Heineken 330ml", 430000, 22, 10, Ngay(11, 10, 2026)),
        MatHang("MH019", "Thung 24 lon Bia Tiger Crystal", 395000, 30, 10, Ngay(12, 10, 2026)),
        MatHang("MH020", "Thung 24 chai Nuoc khoang LaVie", 96000, 42, 15, Ngay(12, 10, 2026)),
        MatHang("MH021", "Banh quy bo Cosy Marie 288g", 45000, 25, 10, Ngay(13, 10, 2026)),
        MatHang("MH022", "Banh Chocopie Orion hop 12 cai", 55000, 32, 10, Ngay(13, 10, 2026)),
        MatHang("MH023", "Keo sua Alpenliebe goi 120g", 20000, 50, 20, Ngay(14, 10, 2026)),
        MatHang("MH024", "Snack tom cay Oishi goi 40g", 6000, 95, 30, Ngay(14, 10, 2026)),
        MatHang("MH025", "Xuc xich tiet trung Vissan 200g", 38000, 12, 15, Ngay(15, 10, 2026)),
        MatHang("MH026", "Thit heo hop Vissan 170g", 42000, 18, 8, Ngay(15, 10, 2026)),
        MatHang("MH027", "Ca ngu ngam dau Ha Long 175g", 28000, 20, 10, Ngay(16, 10, 2026)),
        MatHang("MH028", "Dau goi Clear Mat Lanh 630g", 145000, 24, 10, Ngay(16, 10, 2026)),
        MatHang("MH029", "Dau goi Sunsilk Mem Muot 650g", 135000, 5, 8, Ngay(17, 10, 2026)),
        MatHang("MH030", "Sua tam Lifebuoy Bao Ve 850g", 160000, 30, 10, Ngay(17, 10, 2026)),
        MatHang("MH031", "Kem danh rang P/S Ngua Sau Rang", 43000, 40, 15, Ngay(18, 10, 2026)),
        MatHang("MH032", "Kem danh rang Colgate MaxFresh", 46000, 35, 15, Ngay(18, 10, 2026)),
        MatHang("MH033", "Ban chai danh rang P/S long mem", 22000, 60, 20, Ngay(18, 10, 2026)),
        MatHang("MH034", "Bot giat OMO Matic tui 3kg", 140000, 25, 10, Ngay(19, 10, 2026)),
        MatHang("MH035", "Nuoc giat Ariel Cua Truoc 2.4kg", 175000, 6, 8, Ngay(19, 10, 2026)),
        MatHang("MH036", "Nuoc xa vai Downy Huong Hoa 1.5L", 120000, 28, 10, Ngay(19, 10, 2026)),
        MatHang("MH037", "Nuoc rua chen Sunlight Chanh 750g", 36000, 45, 15, Ngay(20, 10, 2026)),
        MatHang("MH038", "Nuoc lau san Sunlight Huong Hoa 1kg", 32000, 40, 15, Ngay(20, 10, 2026)),
        MatHang("MH039", "Giay ve sinh Bless You 10 cuon", 58000, 35, 12, Ngay(20, 10, 2026)),
        MatHang("MH040", "Khan giay rut Pulppy hop 180 to", 26000, 50, 20, Ngay(20, 10, 2026)),
        MatHang("MH041", "Bang ve sinh Diana Sensi goi 8m", 23000, 40, 15, Ngay(21, 10, 2026)),
        MatHang("MH042", "Ta quan Huggies Dry bich size L", 290000, 4, 6, Ngay(21, 10, 2026)),
        MatHang("MH043", "Sua bot Ensure Gold lon 850g", 780000, 2, 5, Ngay(21, 10, 2026)),
        MatHang("MH044", "Loc 4 hop Sua chua Vinamilk", 28000, 36, 12, Ngay(21, 10, 2026)),
        MatHang("MH045", "Pho mai Con Bo Cuoi hop 8 mieng", 42000, 30, 10, Ngay(21, 10, 2026)),
        MatHang("MH046", "Thung mi Omachi xot bo ham", 215000, 14, 8, Ngay(22, 10, 2026)),
        MatHang("MH047", "Dau hao Maggi chai 350g", 32000, 25, 10, Ngay(22, 10, 2026)),
        MatHang("MH048", "Xot Mayonnaise Kewpie chai 250g", 55000, 18, 6, Ngay(22, 10, 2026)),
        MatHang("MH049", "Banh mi sandwich Otto goi 300g", 20000, 5, 10, Ngay(22, 10, 2026)),
        MatHang("MH050", "Vi 10 qua trung ga tuoi Ba Huan", 36000, 25, 10, Ngay(22, 10, 2026))
    };

    int dem = 0;
    for (int i = 0; i < 50; ++i) {
        if (kho.insert(string(dsMau[i].maHang), dsMau[i])) {
            ++dem;
        }
    }
    return dem;
}

int napDuLieuMauTuFile(KhoHang& kho, const string& tenFile) {
    ifstream file(tenFile.c_str());
    if (!file.is_open()) {
        return khoiTaoDuLieuMau50MatHang(kho);
    }

    string dong;
    int dem = 0;

    if (!getline(file, dong)) {
        file.close();
        return khoiTaoDuLieuMau50MatHang(kho);
    }

    while (getline(file, dong)) {
        if (!dong.empty() && dong.back() == '\r') {
            dong.pop_back();
        }
        if (dong.empty()) continue;

        stringstream ss(dong);
        string ma, ten, giaStr, slTonStr, tonMinStr, ngayStr, thangStr, namStr;

        if (!getline(ss, ma, ',')) continue;
        if (!getline(ss, ten, ',')) continue;
        if (!getline(ss, giaStr, ',')) continue;
        if (!getline(ss, slTonStr, ',')) continue;
        if (!getline(ss, tonMinStr, ',')) continue;
        if (!getline(ss, ngayStr, ',')) continue;
        if (!getline(ss, thangStr, ',')) continue;
        if (!getline(ss, namStr, ',')) continue;

        if (ma.empty() || ma.length() > 15) continue;

        try {
            long long gia = stoll(giaStr);
            int slTon = stoi(slTonStr);
            int tonMin = stoi(tonMinStr);
            Ngay nn(stoi(ngayStr), stoi(thangStr), stoi(namStr));

            MatHang mh(ma.c_str(), ten.c_str(), gia, slTon, tonMin, nn);
            if (kho.insert(ma, mh)) {
                ++dem;
            }
        } catch (...) {
            continue;
        }
    }
    file.close();
    return dem;
}

// =====================================================================
// PHẦN 3: NGHIỆP VỤ NHẬP KHO & XUẤT KHO (TASK T30)
// =====================================================================

bool nhapKho(KhoHang& kho, const string& maHang, int soLuongNhap, Ngay ngayNhapMoi) {
    if (soLuongNhap <= 0) {
        cout << "[Lỗi] Số lượng nhập kho phải lớn hơn 0!\n";
        return false;
    }
    MatHang* mh = kho.search(maHang);
    if (mh == nullptr) {
        cout << "[Lỗi] Không tìm thấy mặt hàng có mã '" << maHang << "' để nhập kho!\n";
        return false;
    }
    mh->soLuongTon += soLuongNhap;
    if (kiemTraNgayHopLe(ngayNhapMoi)) {
        mh->ngayNhap = ngayNhapMoi;
    }
    return true;
}

bool xuatKho(KhoHang& kho, const string& maHang, int soLuongXuat) {
    if (soLuongXuat <= 0) {
        cout << "[Lỗi] Số lượng xuất kho phải lớn hơn 0!\n";
        return false;
    }
    MatHang* mh = kho.search(maHang);
    if (mh == nullptr) {
        cout << "[Lỗi] Không tìm thấy mặt hàng có mã '" << maHang << "' để xuất kho!\n";
        return false;
    }
    if (soLuongXuat > mh->soLuongTon) {
        cout << "[Lỗi] Không thể xuất kho! Số lượng yêu cầu xuất (" << soLuongXuat
             << ") vượt quá số lượng tồn hiện tại (" << mh->soLuongTon << ")!\n";
        return false;
    }

    mh->soLuongTon -= soLuongXuat;
    if (mh->soLuongTon == 0) {
        cout << "--> [CẢNH BÁO]: Mặt hàng " << mh->maHang << " (" << mh->tenHang << ") đã HẾT HÀNG!\n";
    } else if (mh->soLuongTon <= mh->tonToiThieu) {
        cout << "--> [CẢNH BÁO]: Mặt hàng " << mh->maHang << " (" << mh->tenHang
             << ") chỉ còn " << mh->soLuongTon << " sản phẩm (<= mức tối thiểu " << mh->tonToiThieu << ")!\n";
    }
    return true;
}

// =====================================================================
// PHẦN 4: CẢNH BÁO TỒN KHO TỐI THIỂU (TASK T31)
// =====================================================================

vector<MatHang> layDanhSachCanhBaoTonKho(const KhoHang& kho) {
    vector<MatHang> ds = kho.inorder();
    vector<MatHang> dsCanhBao;
    for (size_t i = 0; i < ds.size(); ++i) {
        if (ds[i].soLuongTon <= ds[i].tonToiThieu) {
            dsCanhBao.push_back(ds[i]);
        }
    }
    return dsCanhBao;
}

void canhBaoTonKhoToiThieu(const KhoHang& kho) {
    vector<MatHang> dsCanhBao = layDanhSachCanhBaoTonKho(kho);
    cout << "\n================ CẢNH BÁO TỒN KHO TỐI THIỂU ================\n";
    if (dsCanhBao.empty()) {
        cout << "--> Tất cả mặt hàng trong kho đều có số lượng tồn trên ngưỡng an toàn.\n";
        return;
    }

    cout << string(108, '-') << "\n";
    cout << left << setw(10) << "Mã hàng"
         << setw(32) << "Tên mặt hàng"
         << right << setw(14) << "Đơn giá (đ)"
         << setw(10) << "SL Tồn"
         << setw(10) << "Tồn min"
         << setw(14) << "Cần nhập thêm"
         << "   " << left << setw(14) << "Trạng thái" << "\n";
    cout << string(108, '-') << "\n";

    for (size_t i = 0; i < dsCanhBao.size(); ++i) {
        const MatHang& mh = dsCanhBao[i];
        int goiYNhap = (mh.tonToiThieu * 2) - mh.soLuongTon;
        if (goiYNhap <= 0) goiYNhap = mh.tonToiThieu;
        const char* trangThai = (mh.soLuongTon == 0) ? "HẾT HÀNG!" : "Sắp hết hàng";

        cout << left << setw(10) << mh.maHang
             << setw(32) << mh.tenHang
             << right << setw(14) << mh.donGia
             << setw(10) << mh.soLuongTon
             << setw(10) << mh.tonToiThieu
             << setw(14) << goiYNhap
             << "   " << left << setw(14) << trangThai << "\n";
    }
    cout << string(108, '-') << "\n";
    cout << "--> Tổng số mặt hàng cần nhập bổ sung: " << dsCanhBao.size() << " mặt hàng.\n";
}

// =====================================================================
// PHẦN 5: CÁC HÀM HIỂN THỊ BẢNG VÀ NHẬP LIỆU TỪ BÀN PHÍM CHO MENU
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
    while (cin >> ma) {
        if (ma.length() <= 15) break;
        cout << "[Lỗi] Mã hàng vượt quá 15 ký tự. Vui lòng nhập lại: ";
    }
    if (cin.eof()) return;

    if (kho.search(ma) != nullptr) {
        cout << "[Lỗi] Mã hàng '" << ma << "' đã tồn tại trong hệ thống!\n";
        return;
    }

    xoaBoDem();
    cout << "Nhập tên mặt hàng: ";
    getline(cin, ten);
    if (cin.eof()) return;

    cout << "Nhập đơn giá (VNĐ): ";
    while (!(cin >> gia) || gia < 0) {
        if (cin.eof()) return;
        xoaBoDem();
        cout << "Đơn giá phải >= 0, nhập lại: ";
    }

    cout << "Nhập số lượng tồn ban đầu: ";
    while (!(cin >> slTon) || slTon < 0) {
        if (cin.eof()) return;
        xoaBoDem();
        cout << "Số lượng tồn phải >= 0, nhập lại: ";
    }

    cout << "Nhập ngưỡng tồn tối thiểu: ";
    while (!(cin >> tonMin) || tonMin < 0) {
        if (cin.eof()) return;
        xoaBoDem();
        cout << "Tồn tối thiểu phải >= 0, nhập lại: ";
    }

    cout << "Nhập ngày nhập (Ngay Thang Nam): ";
    while (!(cin >> nn.ngay >> nn.thang >> nn.nam) || !kiemTraNgayHopLe(nn)) {
        if (cin.eof()) return;
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
    if (cin.eof()) return;

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
    if (cin.eof()) return;

    if (tenMoi.empty()) {
        tenMoi = mh->tenHang;
    }

    cout << "Nhập đơn giá mới (hiện tại " << mh->donGia << "): ";
    while (!(cin >> giaMoi) || giaMoi < 0) {
        if (cin.eof()) return;
        xoaBoDem();
        cout << "Đơn giá phải >= 0, nhập lại: ";
    }

    cout << "Nhập mức tồn tối thiểu mới (hiện tại " << mh->tonToiThieu << "): ";
    while (!(cin >> tonMinMoi) || tonMinMoi < 0) {
        if (cin.eof()) return;
        xoaBoDem();
        cout << "Tồn tối thiểu phải >= 0, nhập lại: ";
    }

    cout << "Nhập ngày nhập mới (Ngay Thang Nam): ";
    while (!(cin >> nnMoi.ngay >> nnMoi.thang >> nnMoi.nam) || !kiemTraNgayHopLe(nnMoi)) {
        if (cin.eof()) return;
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
    if (cin.eof()) return;

    MatHang* mh = kho.search(ma);
    if (mh == nullptr) {
        cout << "[Lỗi] Không tìm thấy mặt hàng có mã '" << ma << "'!\n";
        return;
    }

    cout << "-> Mặt hàng: " << mh->tenHang << " | Tồn kho hiện tại: " << mh->soLuongTon << "\n";
    char xacNhan;
    cout << "Bạn có chắc chắn muốn xóa mặt hàng này? (y/n): ";
    cin >> xacNhan;
    if (cin.eof()) return;

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
    if (cin.eof()) return;

    MatHang* mh = timMatHang(kho, ma);
    if (mh == nullptr) {
        cout << "--> Không tìm thấy mặt hàng có mã '" << ma << "' trong kho!\n";
        return;
    }
    inTieuDeBangKho();
    inMotMatHang(*mh);
    cout << string(95, '-') << "\n";
}

void nhapNhapKho(KhoHang& kho) {
    cout << "\n--- NHẬP KHO BỔ SUNG HÀNG HÓA ---\n";
    string ma;
    int soLuongNhap;
    Ngay nnMoi;

    cout << "Nhập mã mặt hàng cần nhập thêm: ";
    cin >> ma;
    if (cin.eof()) return;

    MatHang* mh = kho.search(ma);
    if (mh == nullptr) {
        cout << "[Lỗi] Không tìm thấy mặt hàng có mã '" << ma << "' trong kho!\n";
        return;
    }

    cout << "-> Mặt hàng: " << mh->tenHang << " | Tồn kho hiện tại: " << mh->soLuongTon << "\n";
    cout << "Nhập số lượng nhập thêm (> 0): ";
    while (!(cin >> soLuongNhap) || soLuongNhap <= 0) {
        if (cin.eof()) return;
        xoaBoDem();
        cout << "Số lượng nhập phải > 0, nhập lại: ";
    }

    cout << "Nhập ngày nhập kho mới (Ngay Thang Nam): ";
    while (!(cin >> nnMoi.ngay >> nnMoi.thang >> nnMoi.nam) || !kiemTraNgayHopLe(nnMoi)) {
        if (cin.eof()) return;
        xoaBoDem();
        cout << "Ngày không hợp lệ, nhập lại (Ngay Thang Nam): ";
    }

    if (nhapKho(kho, ma, soLuongNhap, nnMoi)) {
        cout << "==> Nhập kho thành công! Số lượng tồn mới của '" << mh->tenHang
             << "' là: " << mh->soLuongTon << "\n";
    }
}

void nhapXuatKho(KhoHang& kho) {
    cout << "\n--- XUẤT KHO HÀNG HÓA ---\n";
    string ma;
    int soLuongXuat;

    cout << "Nhập mã mặt hàng cần xuất kho: ";
    cin >> ma;
    if (cin.eof()) return;

    MatHang* mh = kho.search(ma);
    if (mh == nullptr) {
        cout << "[Lỗi] Không tìm thấy mặt hàng có mã '" << ma << "' trong kho!\n";
        return;
    }

    cout << "-> Mặt hàng: " << mh->tenHang << " | Tồn kho hiện tại: " << mh->soLuongTon << "\n";
    if (mh->soLuongTon == 0) {
        cout << "[Lỗi] Mặt hàng này hiện đã hết hàng, không thể xuất kho!\n";
        return;
    }

    cout << "Nhập số lượng cần xuất (> 0): ";
    while (!(cin >> soLuongXuat) || soLuongXuat <= 0) {
        if (cin.eof()) return;
        xoaBoDem();
        cout << "Số lượng xuất phải > 0, nhập lại: ";
    }

    if (xuatKho(kho, ma, soLuongXuat)) {
        cout << "==> Xuất kho thành công! Số lượng tồn còn lại của '" << mh->tenHang
             << "' là: " << mh->soLuongTon << "\n";
    }
}

void nhapTimTheoKhoangGia(const KhoHang& kho) {
    cout << "\n--- TRA CỨU MẶT HÀNG THEO KHOẢNG GIÁ ---\n";
    long long giaTu, giaDen;

    cout << "Nhập mức giá thấp nhất (VNĐ): ";
    while (!(cin >> giaTu) || giaTu < 0) {
        if (cin.eof()) return;
        xoaBoDem();
        cout << "Giá phải >= 0, nhập lại: ";
    }

    cout << "Nhập mức giá cao nhất (VNĐ): ";
    while (!(cin >> giaDen) || giaDen < 0) {
        if (cin.eof()) return;
        xoaBoDem();
        cout << "Giá phải >= 0, nhập lại: ";
    }

    vector<MatHang> ketQua;
    timTheoKhoangGia(kho, giaTu, giaDen, ketQua);
    if (ketQua.empty()) {
        cout << "--> Không có mặt hàng nào trong khoảng giá [" << giaTu << ", " << giaDen << "] VNĐ.\n";
        return;
    }

    cout << "--> Tìm thấy " << ketQua.size() << " mặt hàng trong khoảng giá [" << giaTu << ", " << giaDen << "] VNĐ:\n";
    inTieuDeBangKho();
    for (size_t i = 0; i < ketQua.size(); ++i) {
        inMotMatHang(ketQua[i]);
    }
    cout << string(95, '-') << "\n";
}

void nhapTimTheoKhoangMa(const KhoHang& kho) {
    cout << "\n--- TRA CỨU MẶT HÀNG THEO KHOẢNG MÃ ---\n";
    string maTu, maDen;
    cout << "Nhập mã bắt đầu (vd: MH001): ";
    cin >> maTu;
    if (cin.eof()) return;

    cout << "Nhập mã kết thúc (vd: MH010): ";
    cin >> maDen;
    if (cin.eof()) return;

    vector<MatHang> ketQua;
    timTheoKhoangMa(kho, maTu, maDen, ketQua);
    if (ketQua.empty()) {
        cout << "--> Không có mặt hàng nào trong khoảng mã [" << maTu << ", " << maDen << "].\n";
        return;
    }

    cout << "--> Tìm thấy " << ketQua.size() << " mặt hàng trong khoảng mã [" << maTu << ", " << maDen << "]:\n";
    inTieuDeBangKho();
    for (size_t i = 0; i < ketQua.size(); ++i) {
        inMotMatHang(ketQua[i]);
    }
    cout << string(95, '-') << "\n";
}