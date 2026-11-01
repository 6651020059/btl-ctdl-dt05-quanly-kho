// Các file tạm dùng tiền tố tmp_ để KHÔNG BAO GIỜ ghi đè kho.dat thật của người dùng.
#include "FileIO.h"
#include "AVLTree.h"
#include <cstdlib>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>


#define CHECK(expr) do { \
    if (!(expr)) { \
        std::cerr << "[FAIL] CHECK(" #expr ") at " << __FILE__ << ":" << __LINE__ << "\n"; \
        std::exit(EXIT_FAILURE); \
    } \
} while (0)

static bool bangMH(const MatHang& a, const MatHang& b) {
    return !std::strcmp(a.maHang, b.maHang) && !std::strcmp(a.tenHang, b.tenHang) &&
           a.donGia == b.donGia && a.soLuongTon == b.soLuongTon && a.tonToiThieu == b.tonToiThieu &&
           a.ngayNhap.ngay == b.ngayNhap.ngay && a.ngayNhap.thang == b.ngayNhap.thang && a.ngayNhap.nam == b.ngayNhap.nam;
}
static std::vector<char> docHet(const char* p) {
    FILE* f = std::fopen(p, "rb"); std::fseek(f, 0, SEEK_END); long n = std::ftell(f); std::fseek(f, 0, SEEK_SET);
    std::vector<char> b(n); size_t r = std::fread(b.data(), 1, n, f); (void)r; std::fclose(f); return b;
}
static void ghiRa(const char* p, const char* d, size_t n) { FILE* f = std::fopen(p, "wb"); std::fwrite(d, 1, n, f); std::fclose(f); }

int main() {
    std::vector<MatHang> ds;
    ds.push_back(MatHang("MH001", "Sua tuoi", 25000, 100, 10, Ngay(19, 10, 2026)));
    ds.push_back(MatHang("MH002", "Banh mi", 12000, 50, 5, Ngay(20, 10, 2026)));
    HoaDon hd; std::strcpy(hd.maHoaDon, "HD001"); hd.ngayLap = Ngay(23, 10, 2026);
    std::strcpy(hd.danhSachChiTiet[0].maHang, "MH001"); hd.danhSachChiTiet[0].soLuongMua = 3; hd.danhSachChiTiet[0].donGia = 25000;
    hd.soLuongChiTiet = 1; hd.tongTien = 75000;
    std::vector<HoaDon> dsHD(1, hd);

    std::string loi;
    CHECK(FileIO::ghiFile("tmp_kho.dat", ds, dsHD, loi));
    std::vector<MatHang> ds2; std::vector<HoaDon> hd2;
    CHECK(FileIO::docFile("tmp_kho.dat", ds2, hd2, loi));
    CHECK(ds2.size() == 2 && bangMH(ds[0], ds2[0]) && bangMH(ds[1], ds2[1]));
    CHECK(hd2.size() == 1 && !std::strcmp(hd2[0].maHoaDon, "HD001") && hd2[0].tongTien == 75000 &&
           hd2[0].soLuongChiTiet == 1 && hd2[0].danhSachChiTiet[0].soLuongMua == 3 && hd2[0].ngayLap.nam == 2026);
    std::cout << "[OK] ghi roi doc lai dung\n";

    CHECK(FileIO::ghiFile("tmp_rong.dat", std::vector<MatHang>(), std::vector<HoaDon>(), loi));
    CHECK(FileIO::docFile("tmp_rong.dat", ds2, hd2, loi) && ds2.empty() && hd2.empty());
    std::cout << "[OK] file rong hop le\n";

    CHECK(!FileIO::docFile("tmp_khong_co.dat", ds2, hd2, loi));
    std::cout << "[OK] file thieu: " << loi << "\n";

    std::vector<char> b = docHet("tmp_kho.dat");
    ghiRa("tmp_cut.dat", b.data(), b.size() / 2);
    CHECK(!FileIO::docFile("tmp_cut.dat", ds2, hd2, loi));
    std::cout << "[OK] file bi cat: " << loi << "\n";

    std::vector<char> c = b; c[0] = 'X'; ghiRa("tmp_sai.dat", c.data(), c.size());
    CHECK(!FileIO::docFile("tmp_sai.dat", ds2, hd2, loi));
    std::cout << "[OK] sai magic: " << loi << "\n";

    std::vector<char> d = b;
    size_t off = sizeof(FileIO::FileHeader) + offsetof(MatHang, soLuongTon) + 3; d[off] = (char)0x80;
    ghiRa("tmp_bad.dat", d.data(), d.size());
    CHECK(!FileIO::docFile("tmp_bad.dat", ds2, hd2, loi));
    std::cout << "[OK] du lieu vo ly: " << loi << "\n";

    // ---- Bổ sung: file kho.dat hỏng kiểu 3 byte / rỗng phải bị từ chối và KHÔNG làm đổi dữ liệu đang có ----
    ghiRa("tmp_3byte.dat", "abc", 3);
    CHECK(!FileIO::docFile("tmp_3byte.dat", ds2, hd2, loi));
    std::cout << "[OK] file 3 byte bi tu choi: " << loi << "\n";
    ghiRa("tmp_0byte.dat", "", 0);
    CHECK(!FileIO::docFile("tmp_0byte.dat", ds2, hd2, loi));
    std::cout << "[OK] file 0 byte bi tu choi: " << loi << "\n";

    typedef AVLTree<std::string, MatHang> Kho;
    Kho kho;
    kho.insert("MH001", ds[0]);
    std::vector<HoaDon> dsHDGoc(1, hd);
    CHECK(!FileIO::napDuLieu("tmp_3byte.dat", kho, dsHDGoc, loi));
    CHECK(!FileIO::napDuLieu("tmp_cut.dat",   kho, dsHDGoc, loi));
    CHECK(!FileIO::napDuLieu("tmp_bad.dat",   kho, dsHDGoc, loi));
    CHECK(kho.size() == 1 && kho.search("MH001") != nullptr && dsHDGoc.size() == 1);
    std::cout << "[OK] nap file hong that bai thi kho va hoa don dang co giu nguyen\n";

    // D3: version sai phải bị từ chối.
    std::vector<char> e = b;
    uint32_t versionSai = 999;
    std::memcpy(e.data() + offsetof(FileIO::FileHeader, version), &versionSai, sizeof(versionSai));
    ghiRa("tmp_version.dat", e.data(), e.size());
    CHECK(!FileIO::docFile("tmp_version.dat", ds2, hd2, loi));
    std::cout << "[OK] sai version bi tu choi: " << loi << "\n";

    // D3: hai MatHang trùng khóa trong cùng file là dữ liệu hỏng.
    std::vector<char> f = b;
    const size_t offMH2 = sizeof(FileIO::FileHeader) + sizeof(MatHang);
    std::memset(f.data() + offMH2 + offsetof(MatHang, maHang), 0, sizeof(MatHang::maHang));
    std::memcpy(f.data() + offMH2 + offsetof(MatHang, maHang), "MH001", 5);
    ghiRa("tmp_trungma.dat", f.data(), f.size());
    CHECK(!FileIO::docFile("tmp_trungma.dat", ds2, hd2, loi));
    std::cout << "[OK] trung ma mat hang bi tu choi: " << loi << "\n";

    // D3/D4: hóa đơn có số lượng mua bằng 0 phải bị từ chối.
    std::vector<char> g = b;
    const size_t offHD = sizeof(FileIO::FileHeader) + 2 * sizeof(MatHang);
    const size_t offSL = offHD + offsetof(HoaDon, danhSachChiTiet) + offsetof(ChiTietHoaDon, soLuongMua);
    int soLuongVoLy = 0;
    std::memcpy(g.data() + offSL, &soLuongVoLy, sizeof(soLuongVoLy));
    ghiRa("tmp_hd_vo_ly.dat", g.data(), g.size());
    CHECK(!FileIO::docFile("tmp_hd_vo_ly.dat", ds2, hd2, loi));
    std::cout << "[OK] hoa don co so luong chi tiet vo ly bi tu choi: " << loi << "\n";

    // Ghi file bằng ghiFile phải thay thế nguyên tử: không để lại file .tmp khi thành công
    CHECK(FileIO::ghiFile("tmp_kho.dat", ds, dsHD, loi));
    FILE* tmpSot = std::fopen("tmp_kho.dat.tmp", "rb");
    CHECK(tmpSot == nullptr);
    std::cout << "[OK] ghi thanh cong khong de lai file .tmp\n";

    std::remove("tmp_kho.dat"); std::remove("tmp_rong.dat"); std::remove("tmp_cut.dat");
    std::remove("tmp_sai.dat"); std::remove("tmp_bad.dat");
    std::remove("tmp_3byte.dat"); std::remove("tmp_0byte.dat");
    std::remove("tmp_version.dat"); std::remove("tmp_trungma.dat"); std::remove("tmp_hd_vo_ly.dat");
    return 0;
}
