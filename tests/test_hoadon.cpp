// test_hoadon.cpp - Bộ kiểm thử phân hệ Hóa đơn & Doanh thu (TV4, T35-T39, T43)
//
// Biên dịch (từ thư mục gốc repo):
//   g++ -std=c++11 -Wall -Wextra -Iinclude tests/test_hoadon.cpp src/lap_hoa_don.cpp src/thongkedoanhthu.cpp -o test_hoadon
//   ./test_hoadon
//
// Các hàm lapHoaDon / thongKe* đọc từ std::cin và ghi ra std::cout, nên bộ test
// thay tạm bộ đệm của cin/cout bằng stringstream để nạp dữ liệu nhập giả lập và
// bắt lại nội dung in ra. Chương trình trả về 0 nếu mọi ca đều đạt, 1 nếu có ca trượt.

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdio>

#include "lap_hoa_don.h"
#include "thongkedoanhthu.h"

using namespace std;


static int g_dat = 0;
static int g_truot = 0;
static ostream* g_log = &cerr;   // báo kết quả ra stderr để không lẫn với cout bị bắt

static void kiemTra(bool dieuKien, const string& moTa) {
    if (dieuKien) {
        ++g_dat;
        *g_log << "[OK]   " << moTa << "\n";
    } else {
        ++g_truot;
        *g_log << "[FAIL] " << moTa << "\n";
    }
}

// Giả lập bàn phím và bắt màn hình trong phạm vi một lần gọi hàm.
class GiaLapConsole {
public:
    explicit GiaLapConsole(const string& dauVao)
        : in_(dauVao), cu_in_(cin.rdbuf(in_.rdbuf())), cu_out_(cout.rdbuf(out_.rdbuf())) {
        cin.clear();
    }
    ~GiaLapConsole() {
        cin.rdbuf(cu_in_);
        cout.rdbuf(cu_out_);
        cin.clear();
    }
    string dauRa() const { return out_.str(); }
private:
    istringstream in_;
    ostringstream out_;
    streambuf* cu_in_;
    streambuf* cu_out_;
};

static bool coChuoi(const string& s, const string& con) { return s.find(con) != string::npos; }

static void themMH(KhoHang& kho, const char* ma, const char* ten, long long gia, int ton, int tonMin) {
    kho.insert(ma, MatHang(ma, ten, gia, ton, tonMin, Ngay(1, 10, 2026)));
}

static void napKhoMau(KhoHang& kho) {
    themMH(kho, "MH001", "Sua tuoi", 25000, 100, 10);
    themMH(kho, "MH002", "Banh mi", 12000, 50, 5);
    themMH(kho, "MH003", "Nuoc suoi", 6000, 5, 2);     // tồn thấp, dùng thử mua vượt tồn
    themMH(kho, "MH004", "Keo", 3000, 0, 5);           // đã hết hàng
    themMH(kho, "MH005", "Tivi", 5000000000LL, 10, 1); // đơn giá lớn, kiểm tra tràn số int
}

static void lapHoaDonVoiDauVao(KhoHang& kho, vector<HoaDon>& ds, const string& dauVao, string* dauRa = 0) {
    GiaLapConsole io(dauVao);
    lapHoaDon(kho, ds);
    if (dauRa) *dauRa = io.dauRa();
}

static HoaDon taoHoaDon(const char* ma, int d, int m, int y, long long tong) {
    HoaDon hd;
    strncpy(hd.maHoaDon, ma, sizeof(hd.maHoaDon) - 1);
    hd.ngayLap = Ngay(d, m, y);
    hd.tongTien = tong;
    hd.soLuongChiTiet = 1;
    strncpy(hd.danhSachChiTiet[0].maHang, "MH001", sizeof(hd.danhSachChiTiet[0].maHang) - 1);
    hd.danhSachChiTiet[0].soLuongMua = 1;
    hd.danhSachChiTiet[0].donGia = tong;
    return hd;
}

int main() {
    // ---------------------------------------------------------------- Nhóm A
    cerr << "== A. Hàm kiểm tra tồn và trừ tồn ==\n";
    {
        MatHang mh("MH100", "Thu nghiem", 1000, 10, 3, Ngay(1, 1, 2026));
        kiemTra(kiemTraHetHang(mh, 10), "A1: mua đúng bằng tồn (10/10) được chấp nhận");
        kiemTra(!kiemTraHetHang(mh, 11), "A2: mua vượt tồn (11/10) bị từ chối");
        kiemTra(kiemTraHetHang(mh, 1), "A3: mua 1 đơn vị được chấp nhận");

        string dauRa;
        {
            GiaLapConsole io("");
            truTonTuDong(mh, 4);
            dauRa = io.dauRa();
        }
        kiemTra(mh.soLuongTon == 6 && dauRa.empty(), "A4: trừ 4 còn 6, chưa chạm mức tối thiểu nên không cảnh báo");

        {
            GiaLapConsole io("");
            truTonTuDong(mh, 3);   // còn 3 <= tonToiThieu 3
            dauRa = io.dauRa();
        }
        kiemTra(mh.soLuongTon == 3 && coChuoi(dauRa, "tồn kho tối thiểu"), "A5: còn 3 (<= mức tối thiểu 3) thì có cảnh báo tồn tối thiểu");

        {
            GiaLapConsole io("");
            truTonTuDong(mh, 3);   // còn 0
            dauRa = io.dauRa();
        }
        kiemTra(mh.soLuongTon == 0 && coChuoi(dauRa, "HẾT HÀNG"), "A6: trừ về 0 thì có cảnh báo HẾT HÀNG");
    }

    // ---------------------------------------------------------------- Nhóm B
    cerr << "== B. Lập hóa đơn ==\n";
    {
        // B1: hai dòng chi tiết hợp lệ
        KhoHang kho; napKhoMau(kho);
        vector<HoaDon> ds;
        lapHoaDonVoiDauVao(kho, ds, "HD001\n22 10 2026\nMH001\n3\ny\nMH002\n2\nn\ny\n");
        kiemTra(ds.size() == 1, "B1: lập thành công 1 hóa đơn");
        if (ds.size() == 1) {
            const HoaDon& hd = ds[0];
            kiemTra(strcmp(hd.maHoaDon, "HD001") == 0, "B1: mã hóa đơn là HD001");
            kiemTra(hd.ngayLap.ngay == 22 && hd.ngayLap.thang == 10 && hd.ngayLap.nam == 2026, "B1: ngày lập 22/10/2026");
            kiemTra(hd.soLuongChiTiet == 2, "B1: có 2 dòng chi tiết");
            kiemTra(hd.tongTien == 3LL * 25000 + 2LL * 12000, "B1: tổng tiền = 3*25.000 + 2*12.000 = 99.000");
            kiemTra(strcmp(hd.danhSachChiTiet[0].maHang, "MH001") == 0 && hd.danhSachChiTiet[0].soLuongMua == 3 &&
                    hd.danhSachChiTiet[0].donGia == 25000, "B1: dòng 1 ghi đúng mã, số lượng, đơn giá");
        }
        kiemTra(kho.search("MH001")->soLuongTon == 97 && kho.search("MH002")->soLuongTon == 48,
                "B1: tồn kho được trừ tự động (MH001: 100->97, MH002: 50->48)");
    }
    {
        // B2: mua vượt tồn -> bị chặn, tồn không đổi, không có hóa đơn
        KhoHang kho; napKhoMau(kho);
        vector<HoaDon> ds; string out;
        lapHoaDonVoiDauVao(kho, ds, "HD002\n22 10 2026\nMH003\n10\nn\n", &out);
        kiemTra(ds.empty(), "B2: mua 10 khi tồn 5 -> không tạo hóa đơn");
        kiemTra(kho.search("MH003")->soLuongTon == 5, "B2: tồn MH003 giữ nguyên 5");
        kiemTra(coChuoi(out, "Kho không đủ hàng"), "B2: có thông báo kho không đủ hàng");
    }
    {
        // B3: mã hàng không tồn tại
        KhoHang kho; napKhoMau(kho);
        vector<HoaDon> ds; string out;
        lapHoaDonVoiDauVao(kho, ds, "HD003\n22 10 2026\nMH999\nn\n", &out);
        kiemTra(ds.empty(), "B3: mã hàng không tồn tại -> không tạo hóa đơn");
        kiemTra(coChuoi(out, "không tồn tại"), "B3: có thông báo mã hàng không tồn tại");
    }
    {
        // B4: mặt hàng đã hết hàng (tồn 0)
        KhoHang kho; napKhoMau(kho);
        vector<HoaDon> ds; string out;
        lapHoaDonVoiDauVao(kho, ds, "HD004\n22 10 2026\nMH004\nn\n", &out);
        kiemTra(ds.empty(), "B4: mặt hàng tồn 0 -> không tạo hóa đơn");
        kiemTra(coChuoi(out, "hết hàng"), "B4: có cảnh báo mặt hàng đã hết hàng");
    }
    {
        // B5b: ngày không có trong lịch (31/02, 29/02 năm thường) bị từ chối; 29/02 năm nhuận được nhận
        KhoHang kho; napKhoMau(kho);
        vector<HoaDon> ds; string out;
        lapHoaDonVoiDauVao(kho, ds, "HD055\n31 2 2026\n29 2 2027\n31 4 2026\n29 2 2028\nMH001\n1\nn\ny\n", &out);
        kiemTra(coChuoi(out, "Ngày không hợp lệ"), "B5b: 31/02/2026, 29/02/2027, 31/04/2026 bị từ chối");
        kiemTra(ds.size() == 1 && ds[0].ngayLap.ngay == 29 && ds[0].ngayLap.thang == 2 && ds[0].ngayLap.nam == 2028,
                "B5b: 29/02/2028 (năm nhuận) được chấp nhận");
    }
    {
        // B5: ngày sai rồi nhập lại ngày đúng
        KhoHang kho; napKhoMau(kho);
        vector<HoaDon> ds; string out;
        lapHoaDonVoiDauVao(kho, ds, "HD005\n32 13 2026\n23 10 2026\nMH001\n1\nn\ny\n", &out);
        kiemTra(coChuoi(out, "Ngày không hợp lệ"), "B5: ngày 32/13/2026 bị từ chối");
        kiemTra(ds.size() == 1 && ds[0].ngayLap.ngay == 23 && ds[0].ngayLap.thang == 10 && ds[0].ngayLap.nam == 2026,
                "B5: sau khi nhập lại, hóa đơn ghi đúng ngày 23/10/2026");
    }
    {
        // B6: số lượng không hợp lệ (0, âm, chữ) rồi nhập số đúng
        KhoHang kho; napKhoMau(kho);
        vector<HoaDon> ds; string out;
        lapHoaDonVoiDauVao(kho, ds, "HD006\n22 10 2026\nMH001\n0\n-3\nabc\n5\nn\ny\n", &out);
        kiemTra(coChuoi(out, "số nguyên dương"), "B6: số lượng 0, âm, chữ đều bị từ chối");
        kiemTra(ds.size() == 1 && ds[0].danhSachChiTiet[0].soLuongMua == 5 && ds[0].tongTien == 5LL * 25000,
                "B6: cuối cùng ghi nhận số lượng 5, tổng tiền 125.000");
        kiemTra(kho.search("MH001")->soLuongTon == 95, "B6: tồn MH001 còn 95");
    }
    {
        // B7: mã hóa đơn quá dài rồi nhập lại
        KhoHang kho; napKhoMau(kho);
        vector<HoaDon> ds; string out;
        lapHoaDonVoiDauVao(kho, ds, "HD_QUA_DAI_HON_15_KY_TU\nHD007\n22 10 2026\nMH001\n1\nn\ny\n", &out);
        kiemTra(coChuoi(out, "từ 1..15 ký tự"), "B7: mã hóa đơn dài hơn 15 ký tự bị từ chối");
        kiemTra(ds.size() == 1 && strcmp(ds[0].maHoaDon, "HD007") == 0, "B7: nhập lại mã hợp lệ HD007 thì lập được");
    }
    {
        // B8: đơn giá lớn, tổng tiền vượt phạm vi int
        KhoHang kho; napKhoMau(kho);
        vector<HoaDon> ds;
        lapHoaDonVoiDauVao(kho, ds, "HD008\n22 10 2026\nMH005\n3\nn\ny\n");
        kiemTra(ds.size() == 1 && ds[0].tongTien == 15000000000LL, "B8: 3 x 5.000.000.000 = 15.000.000.000 (không tràn số)");
    }
    {
        // B9: kho rỗng
        KhoHang kho;
        vector<HoaDon> ds; string out;
        lapHoaDonVoiDauVao(kho, ds, "HD009\n22 10 2026\n", &out);
        kiemTra(ds.empty() && coChuoi(out, "Kho hàng hiện đang trống"), "B9: kho rỗng -> báo lỗi, không tạo hóa đơn");
    }
    {
        // B10: đạt giới hạn tối đa 20 dòng chi tiết
        KhoHang kho;
        for (int i = 1; i <= 25; ++i) {
            char ma[16]; snprintf(ma, sizeof(ma), "SP%03d", i);
            themMH(kho, ma, "Hang loat", 1000, 100, 1);
        }
        string vao = "HD010\n22 10 2026\n";
        for (int i = 1; i <= 20; ++i) {
            char dong[32]; snprintf(dong, sizeof(dong), "SP%03d\n1\n", i);
            vao += dong;
            if (i < 20) vao += "y\n";
        }
        vao += "y\n"; // xác nhận toàn bộ hóa đơn sau khi đủ 20 dòng
        vector<HoaDon> ds; string out;
        lapHoaDonVoiDauVao(kho, ds, vao, &out);
        kiemTra(ds.size() == 1 && ds[0].soLuongChiTiet == MAX_CHI_TIET, "B10: hóa đơn dừng đúng ở 20 dòng chi tiết");
        kiemTra(ds.size() == 1 && ds[0].tongTien == 20000, "B10: tổng tiền 20 x 1.000 = 20.000");
        kiemTra(coChuoi(out, "giới hạn tối đa"), "B10: có thông báo đạt giới hạn tối đa");
        kiemTra(kho.search("SP021")->soLuongTon == 100, "B10: mặt hàng thứ 21 trở đi không bị trừ tồn");
    }
    {
        // B11: nhiều hóa đơn liên tiếp cùng trừ vào một mặt hàng
        KhoHang kho; napKhoMau(kho);
        vector<HoaDon> ds;
        lapHoaDonVoiDauVao(kho, ds, "HD011\n22 10 2026\nMH003\n3\nn\ny\n");
        lapHoaDonVoiDauVao(kho, ds, "HD012\n22 10 2026\nMH003\n3\nn\ny\n");   // chỉ còn 2, phải bị chặn
        kiemTra(ds.size() == 1 && kho.search("MH003")->soLuongTon == 2,
                "B11: hóa đơn thứ hai mua 3 khi chỉ còn 2 bị chặn, tồn giữ 2");
    }

    // A1/B1: giao dịch phải hủy an toàn khi EOF, người dùng từ chối hoặc mã hóa đơn trùng.
    {
        KhoHang kho; napKhoMau(kho);
        vector<HoaDon> ds; string out;
        lapHoaDonVoiDauVao(kho, ds, "HD013\n22 10 2026\nMH001\n4\n", &out);
        kiemTra(ds.empty() && kho.search("MH001")->soLuongTon == 100 && coChuoi(out, "EOF"),
                "A1: EOF khi nhập hóa đơn hủy giao dịch và giữ nguyên tồn kho");
    }
    {
        KhoHang kho; napKhoMau(kho);
        vector<HoaDon> ds; string out;
        lapHoaDonVoiDauVao(kho, ds, "HD014\n22 10 2026\nMH001\n4\nn\nn\n", &out);
        kiemTra(ds.empty() && kho.search("MH001")->soLuongTon == 100 && coChuoi(out, "Đã hủy hóa đơn"),
                "A1: không xác nhận thì hóa đơn bị hủy, tồn kho không đổi");
    }
    {
        KhoHang kho; napKhoMau(kho);
        vector<HoaDon> ds; ds.push_back(taoHoaDon("HD015", 1, 1, 2026, 25000));
        string out;
        lapHoaDonVoiDauVao(kho, ds, "HD015\nHD016\n22 10 2026\nMH001\n1\nn\ny\n", &out);
        kiemTra(ds.size() == 2 && strcmp(ds[1].maHoaDon, "HD016") == 0 && coChuoi(out, "đã tồn tại"),
                "B1: từ chối mã hóa đơn trùng và nhận mã mới");
    }
    {
        KhoHang kho; napKhoMau(kho);
        vector<HoaDon> ds; string out;
        lapHoaDonVoiDauVao(kho, ds, "HD017\n22 10 2026\nMH003\n3\ny\nMH003\n3\nn\ny\n", &out);
        kiemTra(ds.size() == 1 && ds[0].tongTien == 18000 && kho.search("MH003")->soLuongTon == 2 &&
                coChuoi(out, "Đã chọn: 3"),
                "B6: kiểm tra tồn cộng dồn khi cùng mã hàng xuất hiện nhiều dòng");
    }

    // ---------------------------------------------------------------- Nhóm C
    cerr << "== C. Hiển thị danh sách hóa đơn ==\n";
    {
        vector<HoaDon> rong;
        string out;
        { GiaLapConsole io(""); hienThiDanhSachHoaDon(rong); out = io.dauRa(); }
        kiemTra(coChuoi(out, "Chưa có hóa đơn nào"), "C1: danh sách rỗng in thông báo chưa có hóa đơn");

        vector<HoaDon> ds;
        ds.push_back(taoHoaDon("HD100", 5, 3, 2026, 75000));
        { GiaLapConsole io(""); hienThiDanhSachHoaDon(ds); out = io.dauRa(); }
        kiemTra(coChuoi(out, "HD100") && coChuoi(out, "05/03/2026") && coChuoi(out, "75000") && coChuoi(out, "Tổng số hóa đơn: 1"),
                "C2: in đúng mã, ngày (05/03/2026), tổng tiền và tổng số hóa đơn");
    }

    // ---------------------------------------------------------------- Nhóm D
    cerr << "== D. Thống kê doanh thu ==\n";
    vector<HoaDon> dsTK;
    dsTK.push_back(taoHoaDon("HD201", 22, 10, 2026, 100000));
    dsTK.push_back(taoHoaDon("HD202", 22, 10, 2026, 250000));
    dsTK.push_back(taoHoaDon("HD203", 23, 10, 2026, 40000));
    dsTK.push_back(taoHoaDon("HD204", 22, 11, 2026, 70000));   // cùng ngày 22 nhưng khác tháng
    dsTK.push_back(taoHoaDon("HD205", 22, 10, 2025, 90000));   // cùng ngày/tháng nhưng khác năm
    {
        string out;
        { GiaLapConsole io("22 10 2026\n"); thongKeDoanhThuTheoNgay(dsTK); out = io.dauRa(); }
        kiemTra(coChuoi(out, "hóa đơn lập trong ngày: 2") && coChuoi(out, "Tổng doanh thu: 350000 VNĐ"),
                "D1: ngày 22/10/2026 có 2 hóa đơn, doanh thu 350.000 (không lẫn tháng/năm khác)");

        { GiaLapConsole io("1 1 2026\n"); thongKeDoanhThuTheoNgay(dsTK); out = io.dauRa(); }
        kiemTra(coChuoi(out, "hóa đơn lập trong ngày: 0") && coChuoi(out, "Tổng doanh thu: 0 VNĐ"),
                "D2: ngày không có hóa đơn cho kết quả 0");

        { GiaLapConsole io("40 1 2026\n22 10 2026\n"); thongKeDoanhThuTheoNgay(dsTK); out = io.dauRa(); }
        kiemTra(coChuoi(out, "Định dạng ngày không hợp lệ") && coChuoi(out, "Tổng doanh thu: 350000 VNĐ"),
                "D3: ngày 40/1/2026 bị từ chối, nhập lại 22/10/2026 cho kết quả đúng");

        { GiaLapConsole io("31 2 2026\n31 4 2026\n22 10 2026\n"); thongKeDoanhThuTheoNgay(dsTK); out = io.dauRa(); }
        kiemTra(coChuoi(out, "Định dạng ngày không hợp lệ") && coChuoi(out, "Tổng doanh thu: 350000 VNĐ"),
                "D3b: ngày 31/02/2026 và 31/04/2026 bị từ chối, nhập lại 22/10/2026 cho kết quả đúng");

        { GiaLapConsole io("10 2026\n"); thongKeDoanhThuTheoThang(dsTK); out = io.dauRa(); }
        kiemTra(coChuoi(out, "hóa đơn lập trong tháng: 3") && coChuoi(out, "Tổng doanh thu: 390000 VNĐ"),
                "D4: tháng 10/2026 có 3 hóa đơn, doanh thu 390.000");

        { GiaLapConsole io("10 2025\n"); thongKeDoanhThuTheoThang(dsTK); out = io.dauRa(); }
        kiemTra(coChuoi(out, "hóa đơn lập trong tháng: 1") && coChuoi(out, "Tổng doanh thu: 90000 VNĐ"),
                "D5: tháng 10/2025 chỉ tính hóa đơn của năm 2025");

        { GiaLapConsole io("13 2026\n10 2026\n"); thongKeDoanhThuTheoThang(dsTK); out = io.dauRa(); }
        kiemTra(coChuoi(out, "Tháng (1-12) hoặc năm không hợp lệ") && coChuoi(out, "Tổng doanh thu: 390000 VNĐ"),
                "D6: tháng 13 bị từ chối, nhập lại 10/2026 cho kết quả đúng");

        vector<HoaDon> rong;
        { GiaLapConsole io("22 10 2026\n"); thongKeDoanhThuTheoNgay(rong); out = io.dauRa(); }
        kiemTra(coChuoi(out, "Tổng doanh thu: 0 VNĐ"), "D7: danh sách hóa đơn rỗng cho doanh thu 0");

        vector<HoaDon> lon;
        lon.push_back(taoHoaDon("HD301", 1, 1, 2026, 9000000000LL));
        lon.push_back(taoHoaDon("HD302", 1, 1, 2026, 9000000000LL));
        { GiaLapConsole io("1 1 2026\n"); thongKeDoanhThuTheoNgay(lon); out = io.dauRa(); }
        kiemTra(coChuoi(out, "Tổng doanh thu: 18000000000 VNĐ"), "D8: cộng dồn 18 tỷ không bị tràn số");

        { GiaLapConsole io("1\n22 10 2026\n2\n10 2026\n9\n0\n"); menuThongKeDoanhThu(dsTK); out = io.dauRa(); }
        kiemTra(coChuoi(out, "Tổng doanh thu: 350000 VNĐ") && coChuoi(out, "Tổng doanh thu: 390000 VNĐ") &&
                coChuoi(out, "Lựa chọn không hợp lệ") && coChuoi(out, "Thoát menu thống kê"),
                "D9: menu thống kê chạy được cả hai chức năng, báo lỗi lựa chọn 9 và thoát bằng 0");
    }

    // ---------------------------------------------------------------- Nhóm E
    cerr << "== E. Tích hợp: lập hóa đơn rồi thống kê ==\n";
    {
        KhoHang kho; napKhoMau(kho);
        vector<HoaDon> ds;
        lapHoaDonVoiDauVao(kho, ds, "HD401\n15 11 2026\nMH001\n2\ny\nMH002\n1\nn\ny\n");   // 2*25000 + 12000 = 62000
        lapHoaDonVoiDauVao(kho, ds, "HD402\n15 11 2026\nMH002\n4\nn\ny\n");                 // 48000
        lapHoaDonVoiDauVao(kho, ds, "HD403\n16 11 2026\nMH001\n1\nn\ny\n");                 // 25000
        string out;
        { GiaLapConsole io("15 11 2026\n"); thongKeDoanhThuTheoNgay(ds); out = io.dauRa(); }
        kiemTra(coChuoi(out, "hóa đơn lập trong ngày: 2") && coChuoi(out, "Tổng doanh thu: 110000 VNĐ"),
                "E1: doanh thu ngày 15/11/2026 = 62.000 + 48.000 = 110.000");
        { GiaLapConsole io("11 2026\n"); thongKeDoanhThuTheoThang(ds); out = io.dauRa(); }
        kiemTra(coChuoi(out, "hóa đơn lập trong tháng: 3") && coChuoi(out, "Tổng doanh thu: 135000 VNĐ"),
                "E2: doanh thu tháng 11/2026 = 135.000");
        kiemTra(kho.search("MH001")->soLuongTon == 97 && kho.search("MH002")->soLuongTon == 45,
                "E3: tồn kho sau 3 hóa đơn: MH001 = 97, MH002 = 45");
    }

    cerr << "\nTONG KET: " << g_dat << " dat, " << g_truot << " truot\n";
    if (g_truot == 0) cerr << "TAT CA TEST HOA DON DAT\n";
    return g_truot == 0 ? 0 : 1;
}
