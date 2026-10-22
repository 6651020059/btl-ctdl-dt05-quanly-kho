#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif

#include "mathang.h"
#include "hoadon.h"
#include "AVLTree_Stub.h"
#include "quan_ly_kho.h"
#include "lap_hoa_don.h"
#include "stub_module.h" // Gọi các hàm tạm

using namespace std;

// Hàm tiện ích
static void xoaBoDem() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

static int docLuaChon(int tu, int den, int macDinh) {
    int x;
    while (true) {
        if (cin >> x) {
            if (x >= tu && x <= den) { xoaBoDem(); return x; }
            cout << "Lựa chọn phải từ " << tu << " đến " << den << ", nhập lại: ";
            xoaBoDem();
        } else {
            if (cin.eof()) return macDinh;
            xoaBoDem();
            cout << "Vui lòng nhập một số, nhập lại: ";
        }
    }
}

static void boNewlineThua() {
    cin.clear();
    if (cin.peek() == '\n') cin.get();
}

static void tamDung() {
    cout << "\nNhấn Enter để tiếp tục...";
    string s;
    getline(cin, s);
}

// Menu chính
int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    KhoHang kho;
    vector<HoaDon> dsHD;

    while (true) {
        cout << "\n=============================================\n"
             << "  QUẢN LÝ HÀNG TỒN KHO & DOANH THU SIÊU THỊ\n"
             << "=============================================\n"
             << "1. Quản lý mặt hàng (TV3)\n"
             << "2. Nhập kho / Xuất kho (TV3 - Chưa xong)\n"
             << "3. Bán hàng & hóa đơn (TV4)\n"
             << "4. Cảnh báo tồn kho tối thiểu (TV3 - Chưa xong)\n"
             << "5. Thống kê doanh thu (TV4 - Chưa xong)\n"
             << "6. Lưu dữ liệu ra file (TV5 - Chưa xong)\n"
             << "7. Đọc dữ liệu từ file (TV5 - Chưa xong)\n"
             << "8. Nạp dữ liệu mẫu\n"
             << "0. Thoát\nChọn: ";
             
        int c = docLuaChon(0, 8, 0);
        switch (c) {
            case 1: 
                // Gọi tạm hàm nhập thêm để minh họa, ở bản đầy đủ sẽ có menu con
                nhapThemMatHang(kho); 
                boNewlineThua();
                tamDung();
                break;
            case 2: nhapXuatKho(kho); tamDung(); break;
            case 3: 
                lapHoaDon(kho, dsHD); 
                boNewlineThua();
                tamDung();
                break;
            case 4: canhBaoTonToiThieu(kho); tamDung(); break;
            case 5: thongKeDoanhThu(dsHD); tamDung(); break;
            case 6: luuFile(kho, dsHD); tamDung(); break;
            case 7: docFile(kho, dsHD); tamDung(); break;
            case 8: napDuLieuMau(kho); tamDung(); break;
            case 0: cout << "Tạm biệt!\n"; return 0;
        }
    }
}
