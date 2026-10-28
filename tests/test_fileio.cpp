#include "FileIO.h"
#include <cassert>
#include <cstddef>
#include <iostream>

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
    assert(FileIO::ghiFile("kho.dat", ds, dsHD, loi));
    std::vector<MatHang> ds2; std::vector<HoaDon> hd2;
    assert(FileIO::docFile("kho.dat", ds2, hd2, loi));
    assert(ds2.size() == 2 && bangMH(ds[0], ds2[0]) && bangMH(ds[1], ds2[1]));
    assert(hd2.size() == 1 && !std::strcmp(hd2[0].maHoaDon, "HD001") && hd2[0].tongTien == 75000 &&
           hd2[0].soLuongChiTiet == 1 && hd2[0].danhSachChiTiet[0].soLuongMua == 3 && hd2[0].ngayLap.nam == 2026);
    std::cout << "[OK] ghi roi doc lai dung\n";

    assert(FileIO::ghiFile("rong.dat", std::vector<MatHang>(), std::vector<HoaDon>(), loi));
    assert(FileIO::docFile("rong.dat", ds2, hd2, loi) && ds2.empty() && hd2.empty());
    std::cout << "[OK] file rong hop le\n";

    assert(!FileIO::docFile("khong_co.dat", ds2, hd2, loi));
    std::cout << "[OK] file thieu: " << loi << "\n";

    std::vector<char> b = docHet("kho.dat");
    ghiRa("cut.dat", b.data(), b.size() / 2);
    assert(!FileIO::docFile("cut.dat", ds2, hd2, loi));
    std::cout << "[OK] file bi cat: " << loi << "\n";

    std::vector<char> c = b; c[0] = 'X'; ghiRa("sai.dat", c.data(), c.size());
    assert(!FileIO::docFile("sai.dat", ds2, hd2, loi));
    std::cout << "[OK] sai magic: " << loi << "\n";

    std::vector<char> d = b;
    size_t off = sizeof(FileIO::FileHeader) + offsetof(MatHang, soLuongTon) + 3; d[off] = (char)0x80;
    ghiRa("bad.dat", d.data(), d.size());
    assert(!FileIO::docFile("bad.dat", ds2, hd2, loi));
    std::cout << "[OK] du lieu vo ly: " << loi << "\n";
    return 0;
}
