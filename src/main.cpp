// main.cpp - T06, T08 (TV1) & T47, T48 (TV5)
// Hệ thống Quản lý Hàng tồn kho & Doanh thu Siêu thị (Đề tài ĐT05)
// Tích hợp đầy đủ: Cây AVL + Quản lý Kho + Hóa đơn & Doanh thu + File nhị phân + Benchmark
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <limits>
#include <stdexcept>

#include "AVLTree.h"
#include "mathang.h"
#include "hoadon.h"
#include "quan_ly_kho.h"
#include "lap_hoa_don.h"
#include "thongkedoanhthu.h"
#include "thongketop.h"
#include "FileIO.h"
#include "Chuongtrinhsinhvadodulieu.h"

using namespace std;

static const string DEFAULT_DATA_FILE = "kho.dat";
static const string DEFAULT_CSV_FILE  = "data/du_lieu_mau.csv";

static void xoaBoDemMain() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

static int docLuaChonMenu(int minVal, int maxVal) {
    int chon;
    while (true) {
        cout << ">> Nhập lựa chọn [" << minVal << "-" << maxVal << "]: ";
        if (cin >> chon) {
            if (chon >= minVal && chon <= maxVal) {
                return chon;
            }
            cout << "[Lỗi] Vui lòng chọn số từ " << minVal << " đến " << maxVal << "!\n";
        } else {
            if (cin.eof()) return 0;
            xoaBoDemMain();
            cout << "[Lỗi] Dữ liệu nhập không phải số nguyên hợp lệ! Vui lòng nhập lại.\n";
        }
    }
}

// Menu con 1: Quản lý danh mục mặt hàng & Kho hàng (TV3)
static void menuQuanLyKho(KhoHang& kho) {
    int chon = -1;
    do {
        cout << "\n+----------------------------------------------------------+\n";
        cout << "|              PHÂN HỆ QUẢN LÝ KHO HÀNG (AVL)              |\n";
        cout << "+----------------------------------------------------------+\n";
        cout << "|  1. Hiển thị toàn bộ mặt hàng (theo mã tăng dần - LNR)   |\n";
        cout << "|  2. Thêm mặt hàng mới vào kho                            |\n";
        cout << "|  3. Sửa thông tin mặt hàng                               |\n";
        cout << "|  4. Xóa mặt hàng khỏi kho                                |\n";
        cout << "|  5. Tìm kiếm mặt hàng theo mã chính xác O(log n)         |\n";
        cout << "|  6. Tra cứu mặt hàng theo khoảng mã (Range Search)       |\n";
        cout << "|  7. Tra cứu mặt hàng theo khoảng giá                     |\n";
        cout << "|  8. Nhập kho bổ sung hàng hóa                            |\n";
        cout << "|  9. Xuất kho hàng hóa (có chặn xuất quá tồn)             |\n";
        cout << "| 10. Xem bảng cảnh báo tồn kho tối thiểu                  |\n";
        cout << "|  0. Quay lại Menu chính                                  |\n";
        cout << "+----------------------------------------------------------+\n";
        cout << " (Trạng thái cây AVL hiện tại: " << kho.size()
             << " mặt hàng | Chiều cao h = " << kho.height() << ")\n";

        chon = docLuaChonMenu(0, 10);
        switch (chon) {
            case 1:  hienThiDanhSachKho(kho); break;
            case 2:  nhapThemMatHang(kho); break;
            case 3:  nhapSuaMatHang(kho); break;
            case 4:  nhapXoaMatHang(kho); break;
            case 5:  nhapTimMatHang(kho); break;
            case 6:  nhapTimTheoKhoangMa(kho); break;
            case 7:  nhapTimTheoKhoangGia(kho); break;
            case 8:  nhapNhapKho(kho); break;
            case 9:  nhapXuatKho(kho); break;
            case 10: canhBaoTonKhoToiThieu(kho); break;
            case 0:  break;
        }
    } while (chon != 0 && !cin.eof());
}

// Menu con 2: Quản lý Hóa đơn & Thống kê Doanh thu (TV4)
static void menuHoaDonDoanhThu(KhoHang& kho, vector<HoaDon>& dsHoaDon) {
    int chon = -1;
    do {
        cout << "\n+----------------------------------------------------------+\n";
        cout << "|           PHÂN HỆ HÓA ĐƠN & THỐNG KÊ DOANH THU           |\n";
        cout << "+----------------------------------------------------------+\n";
        cout << "|  1. Lập hóa đơn bán hàng mới (trừ tồn kho tự động)       |\n";
        cout << "|  2. Xem danh sách các hóa đơn đã lập                     |\n";
        cout << "|  3. Thống kê doanh thu theo Ngày / Tháng                 |\n";
        cout << "|  4. Thống kê Top N mặt hàng bán chạy nhất                |\n";
        cout << "|  0. Quay lại Menu chính                                  |\n";
        cout << "+----------------------------------------------------------+\n";
        cout << " (Tổng số hóa đơn hiện có: " << dsHoaDon.size() << " hóa đơn)\n";

        chon = docLuaChonMenu(0, 4);
        switch (chon) {
            case 1: lapHoaDon(kho, dsHoaDon); break;
            case 2: hienThiDanhSachHoaDon(dsHoaDon); break;
            case 3: menuThongKeDoanhThu(dsHoaDon); break;
            case 4: thongKeTopMatHangBanChay(dsHoaDon, kho); break;
            case 0: break;
        }
    } while (chon != 0 && !cin.eof());
}

// Menu con 3: Lưu trữ File nhị phân & Dữ liệu mẫu CSV (TV5 & TV3)
static void menuLuuTruFile(KhoHang& kho, vector<HoaDon>& dsHoaDon) {
    int chon = -1;
    do {
        cout << "\n+----------------------------------------------------------+\n";
        cout << "|          PHÂN HỆ LƯU TRỮ FILE NHỊ PHÂN & DỮ LIỆU         |\n";
        cout << "+----------------------------------------------------------+\n";
        cout << "|  1. Lưu toàn bộ kho & hóa đơn ra file nhị phân (kho.dat) |\n";
        cout << "|  2. Đọc dữ liệu kho & hóa đơn từ file nhị phân (kho.dat) |\n";
        cout << "|  3. Nạp bộ dữ liệu mẫu 50 mặt hàng từ du_lieu_mau.csv    |\n";
        cout << "|  4. Xóa toàn bộ dữ liệu trong bộ nhớ (Reset)             |\n";
        cout << "|  0. Quay lại Menu chính                                  |\n";
        cout << "+----------------------------------------------------------+\n";

        chon = docLuaChonMenu(0, 4);
        string loi;
        switch (chon) {
            case 1: {
                if (FileIO::luuDuLieu(DEFAULT_DATA_FILE, kho, dsHoaDon, loi)) {
                    cout << "==> [THÀNH CÔNG] Đã lưu " << kho.size() << " mặt hàng và "
                         << dsHoaDon.size() << " hóa đơn vào file '" << DEFAULT_DATA_FILE << "'!\n";
                } else {
                    cout << "==> [LỖI GHI FILE] " << loi << "\n";
                }
                break;
            }
            case 2: {
                if (FileIO::napDuLieu(DEFAULT_DATA_FILE, kho, dsHoaDon, loi)) {
                    cout << "==> [THÀNH CÔNG] Đã nạp " << kho.size() << " mặt hàng và "
                         << dsHoaDon.size() << " hóa đơn từ file '" << DEFAULT_DATA_FILE << "'!\n";
                } else {
                    cout << "==> [LỖI ĐỌC FILE] " << loi << " (Dữ liệu hiện tại được giữ nguyên).\n";
                }
                break;
            }
            case 3: {
                int soNap = napDuLieuMauTuFile(kho, DEFAULT_CSV_FILE);
                cout << "==> Đã nạp thêm " << soNap << " mặt hàng mẫu vào kho! (Tổng trong kho: "
                     << kho.size() << " mặt hàng, chiều cao cây AVL = " << kho.height() << ")\n";
                break;
            }
            case 4: {
                char xacNhan;
                cout << "Bạn có chắc chắn muốn xóa trắng toàn bộ kho và hóa đơn trong RAM? (y/n): ";
                if (cin >> xacNhan && (xacNhan == 'y' || xacNhan == 'Y')) {
                    kho.clear();
                    dsHoaDon.clear();
                    cout << "==> Đã giải phóng toàn bộ cây AVL và danh sách hóa đơn!\n";
                } else {
                    cout << "==> Đã hủy thao tác xóa trắng.\n";
                }
                break;
            }
            case 0:
                break;
        }
    } while (chon != 0 && !cin.eof());
}

int main() {
    // Task T48: Bọc khối try/catch xử lý ngoại lệ an toàn và tự động thu hồi bộ nhớ qua Destructor
    try {
        KhoHang kho;
        vector<HoaDon> dsHoaDon;

        // Tự động nạp file nhị phân kho.dat nếu có sẵn; nếu chưa có thì nạp 50 mặt hàng mẫu từ CSV
        string loi;
        if (FileIO::napDuLieu(DEFAULT_DATA_FILE, kho, dsHoaDon, loi)) {
            cout << "[Khởi động] Đã tự động khôi phục " << kho.size() << " mặt hàng và "
                 << dsHoaDon.size() << " hóa đơn từ '" << DEFAULT_DATA_FILE << "'.\n";
        } else {
            int dem = napDuLieuMauTuFile(kho, DEFAULT_CSV_FILE);
            cout << "[Khởi động] Chưa có '" << DEFAULT_DATA_FILE
                 << "' -> Đã nạp sẵn " << dem << " mặt hàng mẫu (Chiều cao cây AVL: "
                 << kho.height() << ").\n";
        }

        int luaChon = -1;
        do {
            cout << "\n============================================================\n";
            cout << "   HỆ THỐNG QUẢN LÝ KHO HÀNG & DOANH THU SIÊU THỊ (AVL)     \n";
            cout << "============================================================\n";
            cout << "  1. Phân hệ Quản lý Kho hàng (CRUD, Nhập/Xuất, Cảnh báo)\n";
            cout << "  2. Phân hệ Hóa đơn & Doanh thu (Bán hàng, Thống kê, Top)\n";
            cout << "  3. Phân hệ Lưu trữ File nhị phân (kho.dat) & Nạp mẫu CSV\n";
            cout << "  4. Chạy Benchmark đo hiệu năng AVL vs Vector (Task T40)\n";
            cout << "  5. Xem nhanh Cảnh báo tồn kho tối thiểu\n";
            cout << "  0. Lưu dữ liệu và Thoát chương trình\n";
            cout << "------------------------------------------------------------\n";
            cout << "  [Kho: " << kho.size() << " SP | Cao AVL: " << kho.height()
                 << " | Hóa đơn: " << dsHoaDon.size() << "]\n";
            cout << "============================================================\n";

            luaChon = docLuaChonMenu(0, 5);
            switch (luaChon) {
                case 1:
                    menuQuanLyKho(kho);
                    break;
                case 2:
                    menuHoaDonDoanhThu(kho, dsHoaDon);
                    break;
                case 3:
                    menuLuuTruFile(kho, dsHoaDon);
                    break;
                case 4:
                    chayToanBoBenchmark();
                    break;
                case 5:
                    canhBaoTonKhoToiThieu(kho);
                    break;
                case 0: {
                    string loiLuu;
                    if (FileIO::luuDuLieu(DEFAULT_DATA_FILE, kho, dsHoaDon, loiLuu)) {
                        cout << "==> Đã tự động lưu dữ liệu vào '" << DEFAULT_DATA_FILE << "'.\n";
                    }
                    cout << "==> Cảm ơn bạn đã sử dụng Hệ thống Quản lý Siêu thị! Hẹn gặp lại.\n";
                    break;
                }
            }
        } while (luaChon != 0 && !cin.eof());

    } catch (const exception& ex) {
        cerr << "[Ngoại lệ hệ thống]: " << ex.what() << "\n";
        return 1;
    } catch (...) {
        cerr << "[Ngoại lệ không xác định]: Chương trình kết thúc an toàn.\n";
        return 1;
    }
    return 0;
}
