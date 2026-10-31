// test_kho.cpp - Kiểm thử tích hợp Nghiệp vụ Kho (T28-T34) & Lưu/Nạp FileIO trên AVLTree thật
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <vector>
#include "AVLTree.h"
#include "quan_ly_kho.h"
#include "FileIO.h"

#define CHECK(expr) do { \
    if (!(expr)) { \
        std::cerr << "[FAIL] CHECK(" #expr ") at " << __FILE__ << ":" << __LINE__ << "\n"; \
        std::exit(EXIT_FAILURE); \
    } \
} while (0)


int main() {
    KhoHang kho;

    // 1. Kiểm thử nạp 50 mặt hàng mẫu (T29)
    int soNap = napDuLieuMauTuFile(kho, "du_lieu_mau.csv");
    CHECK(soNap == 50 && kho.size() == 50);
    CHECK(kho.height() <= 8);
    std::cout << "[OK] T29: Nap 50 mat hang mau vao AVLTree that (height = " << kho.height() << ")\n";

    // 2. Kiểm thử CRUD (T28)
    CHECK(themMatHang(kho, MatHang("MH051", "Nuoc ngot 7Up 330ml", 10000, 30, 10, Ngay(25, 10, 2026))));
    CHECK(kho.size() == 51);
    CHECK(!themMatHang(kho, MatHang("MH051", "Trung ma", 10000, 10, 5, Ngay(25, 10, 2026))));

    CHECK(suaMatHang(kho, "MH051", "Nuoc ngot 7Up Lon 330ml", 11000, 12, Ngay(26, 10, 2026)));
    MatHang* mh51 = timMatHang(kho, "MH051");
    CHECK(mh51 != nullptr && mh51->donGia == 11000 && mh51->tonToiThieu == 12);

    CHECK(xoaMatHang(kho, "MH051"));
    CHECK(timMatHang(kho, "MH051") == nullptr && kho.size() == 50);
    std::cout << "[OK] T28: 4 thao tac CRUD (Them, Sua, Xoa, Tim) tren AVLTree that\n";

    // 3. Kiểm thử Nhập kho & Xuất kho chặn xuất quá tồn (T30)
    MatHang* mh01 = timMatHang(kho, "MH001");
    CHECK(mh01 != nullptr);
    int tonBanDau = mh01->soLuongTon;
    CHECK(nhapKho(kho, "MH001", 20, Ngay(25, 10, 2026)));
    CHECK(mh01->soLuongTon == tonBanDau + 20);

    CHECK(!xuatKho(kho, "MH001", 999)); // Chặn xuất quá tồn
    CHECK(mh01->soLuongTon == 70);
    CHECK(xuatKho(kho, "MH001", 65));   // Xuất hợp lệ, còn 5 (<= tồn tối thiểu 10 -> in cảnh báo)
    CHECK(mh01->soLuongTon == 5);
    std::cout << "[OK] T30: Nhap kho, Xuat kho va Chan xuat qua ton\n";

    // 3b. Kiểm thử biên: chấp nhận vừa đủ INT_MAX, từ chối phép nhập gây tràn số.
    KhoHang khoBien;
    const int INT_MAX_VALUE = std::numeric_limits<int>::max();
    CHECK(khoBien.insert("MH999", MatHang("MH999", "Hang kiem thu tran so",
                                           1000, INT_MAX_VALUE - 2, 0,
                                           Ngay(25, 10, 2026))));
    MatHang* mhBien = khoBien.search("MH999");
    CHECK(mhBien != nullptr);
    CHECK(nhapKho(khoBien, "MH999", 2, Ngay(25, 10, 2026)));
    CHECK(mhBien->soLuongTon == INT_MAX_VALUE);
    CHECK(!nhapKho(khoBien, "MH999", 1, Ngay(26, 10, 2026)));
    CHECK(mhBien->soLuongTon == INT_MAX_VALUE); // Tồn kho không đổi khi bị từ chối.
    std::cout << "[OK] T30: Chan tran so khi nhap kho (INT_MAX boundary)\n";

    // 3c. Kiểm thử biên bổ sung: nhập một lượng rất lớn (INT_MAX) vào hàng đã có tồn > 0 bị từ chối.
    CHECK(!nhapKho(khoBien, "MH999", INT_MAX_VALUE, Ngay(25, 10, 2026)));
    CHECK(mhBien->soLuongTon == INT_MAX_VALUE);
    CHECK(khoBien.insert("MH998", MatHang("MH998", "Hang ton 1", 1000, 1, 0, Ngay(25, 10, 2026))));
    MatHang* mhMot = khoBien.search("MH998");
    CHECK(!nhapKho(khoBien, "MH998", INT_MAX_VALUE, Ngay(25, 10, 2026)));   // 1 + INT_MAX tràn
    CHECK(mhMot->soLuongTon == 1);
    CHECK(nhapKho(khoBien, "MH998", INT_MAX_VALUE - 1, Ngay(25, 10, 2026))); // 1 + (INT_MAX-1) = INT_MAX vừa đủ
    CHECK(mhMot->soLuongTon == INT_MAX_VALUE);
    std::cout << "[OK] T30: Bien tran so voi nhap lon (INT_MAX) va ton nho\n";

    // 3d. Kiểm thử ngày theo LỊCH THỰC TẾ (31/02, 31/04, năm nhuận) - lỗi "ngày tháng chưa kiểm tra lịch"
    CHECK(!ngayHopLe(Ngay(31, 2, 2026)) && !ngayHopLe(Ngay(30, 2, 2028)));
    CHECK(!ngayHopLe(Ngay(31, 4, 2026)) && !ngayHopLe(Ngay(31, 11, 2026)));
    CHECK(!ngayHopLe(Ngay(29, 2, 2027)) && !ngayHopLe(Ngay(29, 2, 2100)));  // 2100 không nhuận
    CHECK(ngayHopLe(Ngay(29, 2, 2028)) && ngayHopLe(Ngay(29, 2, 2000)));    // 2028, 2000 là năm nhuận
    CHECK(ngayHopLe(Ngay(28, 2, 2027)) && ngayHopLe(Ngay(31, 12, 2026)) && ngayHopLe(Ngay(30, 4, 2026)));
    CHECK(!ngayHopLe(Ngay(0, 1, 2026)) && !ngayHopLe(Ngay(1, 0, 2026)) && !ngayHopLe(Ngay(1, 13, 2026)));
    CHECK(!ngayHopLe(Ngay(1, 1, 1899)) && !ngayHopLe(Ngay(1, 1, 2101)));
    CHECK(!ngayHopLe(Ngay()));   // ngày mặc định 0/0/0 không hợp lệ

    // Thêm / sửa / nhập kho với ngày sai lịch phải bị từ chối và KHÔNG làm đổi dữ liệu
    KhoHang khoNgay;
    CHECK(!themMatHang(khoNgay, MatHang("NG001", "Hang ngay sai", 1000, 5, 1, Ngay(31, 2, 2026))));
    CHECK(khoNgay.size() == 0);
    CHECK(themMatHang(khoNgay, MatHang("NG001", "Hang ngay dung", 1000, 5, 1, Ngay(29, 2, 2028))));
    MatHang* mhNg = timMatHang(khoNgay, "NG001");
    CHECK(mhNg != nullptr);
    CHECK(!suaMatHang(khoNgay, "NG001", "Ten moi", 2000, 3, Ngay(31, 4, 2026)));
    CHECK(std::strcmp(mhNg->tenHang, "Hang ngay dung") == 0 && mhNg->donGia == 1000 &&
           mhNg->tonToiThieu == 1 && mhNg->ngayNhap.ngay == 29);   // chưa bị sửa dở dang
    CHECK(!nhapKho(khoNgay, "NG001", 10, Ngay(29, 2, 2027)));
    CHECK(mhNg->soLuongTon == 5 && mhNg->ngayNhap.nam == 2028);   // tồn và ngày không đổi
    CHECK(nhapKho(khoNgay, "NG001", 10, Ngay(28, 2, 2027)));
    CHECK(mhNg->soLuongTon == 15 && mhNg->ngayNhap.ngay == 28 && mhNg->ngayNhap.nam == 2027);
    std::cout << "[OK] Ngay thang theo lich thuc te: 31/02, 31/04, 29/02 (nam nhuan / khong nhuan)\n";

    // 4. Kiểm thử Cảnh báo tồn kho tối thiểu (T31) & Tra cứu khoảng mã / khoảng giá
    std::vector<MatHang> dsCanhBao = layDanhSachCanhBaoTonKho(kho);
    CHECK(!dsCanhBao.empty());
    std::vector<MatHang> dsKhoangMa;
    timTheoKhoangMa(kho, "MH001", "MH005", dsKhoangMa);
    CHECK(dsKhoangMa.size() == 5);
    std::vector<MatHang> dsKhoangGia;
    timTheoKhoangGia(kho, 10000, 20000, dsKhoangGia);
    CHECK(!dsKhoangGia.empty());
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
    CHECK(FileIO::luuDuLieu("kho_test.dat", kho, dsHD, loi));

    KhoHang kho2;
    std::vector<HoaDon> hd2;
    CHECK(FileIO::napDuLieu("kho_test.dat", kho2, hd2, loi));
    CHECK(kho2.size() == 50 && hd2.size() == 1 && hd2[0].tongTien == 96000);
    MatHang* m = kho2.search("MH001");
    CHECK(m && m->soLuongTon == 5 && m->ngayNhap.ngay == 25);
    std::remove("kho_test.dat");
    std::cout << "[OK] T45 & T46: Luu kho AVLTree that -> nap vao kho moi dung\n";
    return 0;
}
