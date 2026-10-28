// test_kho.cpp - Kiểm thử tích hợp Nghiệp vụ Kho (T28-T34) & Lưu/Nạp FileIO trên AVLTree thật
#include <cassert>
#include <cstring>
#include <iostream>
#include <vector>
#include "AVLTree.h"
#include "quan_ly_kho.h"
#include "FileIO.h"

int main() {
    KhoHang kho;

    // 1. Kiểm thử nạp 50 mặt hàng mẫu (T29)
    int soNap = napDuLieuMauTuFile(kho, "data/du_lieu_mau.csv");
    assert(soNap == 50 && kho.size() == 50);
    assert(kho.height() <= 8);
    std::cout << "[OK] T29: Nap 50 mat hang mau vao AVLTree that (height = " << kho.height() << ")\n";

    // 2. Kiểm thử CRUD (T28)
    assert(themMatHang(kho, MatHang("MH051", "Nuoc ngot 7Up 330ml", 10000, 30, 10, Ngay(25, 10, 2026))));
    assert(kho.size() == 51);
    assert(!themMatHang(kho, MatHang("MH051", "Trung ma", 10000, 10, 5, Ngay(25, 10, 2026))));

    assert(suaMatHang(kho, "MH051", "Nuoc ngot 7Up Lon 330ml", 11000, 12, Ngay(26, 10, 2026)));
    MatHang* mh51 = timMatHang(kho, "MH051");
    assert(mh51 != nullptr && mh51->donGia == 11000 && mh51->tonToiThieu == 12);

    assert(xoaMatHang(kho, "MH051"));
    assert(timMatHang(kho, "MH051") == nullptr && kho.size() == 50);
    std::cout << "[OK] T28: 4 thao tac CRUD (Them, Sua, Xoa, Tim) tren AVLTree that\n";

    // 3. Kiểm thử Nhập kho & Xuất kho chặn xuất quá tồn (T30)
    MatHang* mh01 = timMatHang(kho, "MH001");
    assert(mh01 != nullptr);
    int tonBanDau = mh01->soLuongTon;
    assert(nhapKho(kho, "MH001", 20, Ngay(25, 10, 2026)));
    assert(mh01->soLuongTon == tonBanDau + 20);

    assert(!xuatKho(kho, "MH001", 999)); // Chặn xuất quá tồn
    assert(mh01->soLuongTon == 70);
    assert(xuatKho(kho, "MH001", 65));   // Xuất hợp lệ, còn 5 (<= tồn tối thiểu 10 -> in cảnh báo)
    assert(mh01->soLuongTon == 5);
    std::cout << "[OK] T30: Nhap kho, Xuat kho va Chan xuat qua ton\n";

    // 4. Kiểm thử Cảnh báo tồn kho tối thiểu (T31) & Tra cứu khoảng mã / khoảng giá
    std::vector<MatHang> dsCanhBao = layDanhSachCanhBaoTonKho(kho);
    assert(!dsCanhBao.empty());
    std::vector<MatHang> dsKhoangMa;
    timTheoKhoangMa(kho, "MH001", "MH005", dsKhoangMa);
    assert(dsKhoangMa.size() == 5);
    std::vector<MatHang> dsKhoangGia;
    timTheoKhoangGia(kho, 10000, 20000, dsKhoangGia);
    assert(!dsKhoangGia.empty());
    std::cout << "[OK] T31 & T22: Canh bao ton kho (" << dsCanhBao.size()
              << " SP) & Range Search khoang ma / khoang gia\n";

    // 5. Kiểm thử Lưu/Nạp File nhị phân với cây AVL thật (T45, T46)
    std::vector<HoaDon> dsHD(1);
    std::strcpy(dsHD[0].maHoaDon, "HD001");
    dsHD[0].ngayLap = Ngay(25, 10, 2026);
    dsHD[0].soLuongChiTiet = 1;
    std::strcpy(dsHD[0].danhSachChiTiet[0].maHang, "MH001");
    dsHD[0].danhSachChiTiet[0].soLuongMua = 3;
    dsHD[0].danhSachChiTiet[0].donGia = 32000;
    dsHD[0].tongTien = 96000;

    std::string loi;
    assert(FileIO::luuDuLieu("kho_test.dat", kho, dsHD, loi));

    KhoHang kho2;
    std::vector<HoaDon> hd2;
    assert(FileIO::napDuLieu("kho_test.dat", kho2, hd2, loi));
    assert(kho2.size() == 50 && hd2.size() == 1 && hd2[0].tongTien == 96000);
    MatHang* m = kho2.search("MH001");
    assert(m && m->soLuongTon == 5 && m->ngayNhap.ngay == 25);
    std::remove("kho_test.dat");
    std::cout << "[OK] T45 & T46: Luu kho AVLTree that -> nap vao kho moi dung\n";
    return 0;
}
