// T23: Bộ test phép quay, xóa, Range Search
// Tiêu chí xong: test chạy qua hết; kết quả ghi lại để đưa vào báo cáo.
//
// Chạy: ./test_suite            -> in kết quả ra màn hình và ghi vào ket_qua_test.txt
//       ./test_suite duong_dan  -> ghi vào file khác
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <map>
#include <random>
#include <set>
#include <string>
#include <vector>

// Chỉ trong file test: mở private để kiểm tra cấu trúc nội bộ của cây.
#define private public
#include "AVLTree.h"
#undef private

// ------------------------------------------------------------ Khung test đơn giản
// Không dừng khi sai: ghi nhận từng ca đạt/trượt để lập bảng kết quả.
struct Nhom { std::string ten; int dat; int truot; };
static std::vector<Nhom> g_nhom;
static std::vector<std::string> g_loi;
static std::string g_dong;   // nội dung ghi ra file

static void batDau(const std::string& ten) { g_nhom.push_back({ten, 0, 0}); }
static void kiemTra(bool dung, const std::string& moTa) {
    if (dung) ++g_nhom.back().dat;
    else { ++g_nhom.back().truot; g_loi.push_back("[" + g_nhom.back().ten + "] " + moTa); }
}
static void ghi(const char* fmt, ...) __attribute__((format(printf, 1, 2)));
static void ghi(const char* fmt, ...) {
    char buf[1024];
    va_list ap; va_start(ap, fmt); vsnprintf(buf, sizeof buf, fmt, ap); va_end(ap);
    g_dong += buf;
    std::fputs(buf, stdout);
}

// ------------------------------------------------------------ Kiểm tra cấu trúc cây
template <typename K, typename V>
bool hopLe(AVLTree<K, V>& c, AVLNode<K, V>* n, const K* lo, const K* hi, std::set<K>& ds) {
    if (!n) return true;
    if (lo && !(*lo < n->key)) return false;
    if (hi && !(n->key < *hi)) return false;
    if (n->height != 1 + std::max(c.getHeightNode(n->left), c.getHeightNode(n->right))) return false;
    int bf = c.getBalanceFactor(n);
    if (bf < -1 || bf > 1) return false;
    ds.insert(n->key);
    return hopLe(c, n->left, lo, &n->key, ds) && hopLe(c, n->right, &n->key, hi, ds);
}
template <typename K, typename V>
bool canBang(AVLTree<K, V>& c, std::set<K>& ds) {
    ds.clear();
    return hopLe<K, V>(c, c.root, nullptr, nullptr, ds);
}
template <typename K, typename V>
bool canBang(AVLTree<K, V>& c) { std::set<K> ds; return canBang(c, ds); }

typedef AVLTree<int, int> Cay;
// Cây dựng sẵn từ danh sách khóa (dùng lớp con thay vì trả cây theo giá trị,
// vì AVLTree không được sao chép).
struct CayMau : Cay {
    CayMau(std::initializer_list<int> ks) { for (int k : ks) insert(k, k); }
};

// ============================================================ NHÓM 1: PHÉP QUAY
static void testQuay() {
    batDau("Phep quay");
    struct Ca { const char* ten; int a, b, c; };
    const Ca ca[4] = {{"RR", 1, 2, 3}, {"LL", 3, 2, 1}, {"LR", 3, 1, 2}, {"RL", 1, 3, 2}};
    for (auto& t : ca) {
        Cay a;
        a.insert(t.a, t.a); a.insert(t.b, t.b); a.insert(t.c, t.c);
        kiemTra(a.root->key == 2 && a.root->left->key == 1 && a.root->right->key == 3 &&
                a.height() == 2, std::string("quay ") + t.ten + " cho goc = 2");
    }
    // Quay xảy ra ở cây con (không chỉ ở gốc): chèn thêm vào cây 7 nút đã cân bằng
    const std::vector<std::vector<int>> conCa = {
        {50, 30, 70, 20, 40, 60, 80, 10, 5},
        {50, 30, 70, 20, 40, 60, 80, 90, 100},
        {50, 30, 70, 20, 40, 60, 80, 35, 45, 32},
        {50, 30, 70, 20, 40, 60, 80, 65, 62}};
    int stt = 1;
    for (auto& d : conCa) {
        Cay t;
        bool ok = true;
        for (int k : d) { t.insert(k, k); ok = ok && canBang(t); }
        kiemTra(ok, "quay o cay con, ca " + std::to_string(stt++));
    }
    // Tăng dần / giảm dần / ngẫu nhiên: luôn |balance factor| <= 1
    Cay tang, giam, ngau;
    bool okT = true, okG = true, okN = true;
    for (int k = 1; k <= 1000; ++k) { tang.insert(k, k); okT = okT && canBang(tang); }
    for (int k = 1000; k >= 1; --k) { giam.insert(k, k); okG = okG && canBang(giam); }
    std::mt19937 rng(2026);
    for (int i = 0; i < 1000; ++i) { int k = static_cast<int>(rng() % 100000); ngau.insert(k, k); okN = okN && canBang(ngau); }
    kiemTra(okT && tang.height() <= 11, "chen tang dan 1..1000, can bang, chieu cao <= 11");
    kiemTra(okG && giam.height() <= 11, "chen giam dan 1000..1, can bang, chieu cao <= 11");
    kiemTra(okN, "chen 1000 khoa ngau nhien, can bang sau moi lan them");
    kiemTra(ngau.height() <= 1.44 * std::log2(ngau.size() + 2.0), "chieu cao <= 1.44*log2(n+2)");
}

// ============================================================ NHÓM 2: XÓA
static void testXoa() {
    batDau("Xoa node");
    std::set<int> ds;
    // Cây: 4 | 3 6 | 2 _ 5 7 | 1
    { CayMau t({4, 3, 6, 2, 5, 7, 1});
      kiemTra(t.remove(5) && t.size() == 6 && canBang(t, ds) && !ds.count(5), "xoa nut la"); }
    { CayMau t({4, 3, 6, 2, 5, 7, 1});
      kiemTra(t.remove(2) && t.size() == 6 && canBang(t, ds) && !ds.count(2) && ds.count(1), "xoa nut mot con"); }
    { CayMau t({4, 3, 6, 2, 5, 7, 1});
      kiemTra(t.remove(6) && t.size() == 6 && canBang(t, ds) && !ds.count(6), "xoa nut hai con");
      kiemTra(t.remove(4) && t.size() == 5 && canBang(t, ds) && !ds.count(4), "xoa nut goc"); }
    { CayMau t({4, 3, 6});
      kiemTra(!t.remove(99) && t.size() == 3 && canBang(t, ds), "xoa khoa khong ton tai"); }
    { Cay rong;
      kiemTra(!rong.remove(1) && rong.empty(), "xoa tren cay rong"); }
    { CayMau t({2, 1, 3, 4});
      kiemTra(t.remove(1) && canBang(t, ds) && t.root->key == 3, "xoa gay lech phai -> quay RR"); }
    { CayMau t({10, 5, 20, 2, 15, 30, 12, 40});
      kiemTra(t.getBalanceFactor(t.root->right) == 0 && t.remove(2) && canBang(t, ds) && t.root->key == 20,
              "xoa, con co balance factor = 0 -> quay don"); }

    // Xóa liên tiếp 1000 khóa theo thứ tự ngẫu nhiên: cân bằng sau MỖI lần xóa
    std::mt19937 rng(2026);
    Cay cay; std::set<int> mau;
    while (mau.size() < 1000) { int k = static_cast<int>(rng() % 100000); if (mau.insert(k).second) cay.insert(k, k); }
    std::vector<int> thuTu(mau.begin(), mau.end());
    std::shuffle(thuTu.begin(), thuTu.end(), rng);
    bool ok = true;
    for (int k : thuTu) {
        ok = ok && cay.remove(k);
        mau.erase(k);
        ok = ok && canBang(cay, ds) && ds == mau && cay.size() == static_cast<int>(mau.size());
    }
    kiemTra(ok && cay.empty(), "xoa lien tiep 1000 khoa ngau nhien, luon can bang, cay rong o cuoi");

    for (int huong = 0; huong < 2; ++huong) {
        Cay t; for (int k = 1; k <= 500; ++k) t.insert(k, k);
        bool o = true;
        for (int i = 1; i <= 500; ++i) { o = o && t.remove(huong == 0 ? i : 501 - i) && canBang(t); }
        kiemTra(o && t.empty(), huong == 0 ? "xoa tang dan 1..500" : "xoa giam dan 500..1");
    }
    // Trộn thêm/xóa 100.000 thao tác, đối chiếu std::map
    Cay avl; std::map<int, int> ref; bool okMap = true;
    for (int i = 0; i < 100000; ++i) {
        int k = static_cast<int>(rng() % 3000);
        if (rng() % 3 < 2) okMap = okMap && (avl.insert(k, k) == ref.emplace(k, k).second);
        else               okMap = okMap && (avl.remove(k) == (ref.erase(k) > 0));
        okMap = okMap && avl.size() == static_cast<int>(ref.size());
        if (i % 1000 == 0) okMap = okMap && canBang(avl);
    }
    kiemTra(okMap && canBang(avl, ds) && ds.size() == ref.size(), "100000 thao tac them/xoa doi chieu std::map");
}

// ============================================================ NHÓM 3: RANGE SEARCH
static void testRange() {
    batDau("Range Search");
    CayMau t({10, 20, 30, 40, 50, 60, 70});
    std::vector<int> kq;
    t.rangeSearch(25, 55, kq);
    kiemTra(kq == std::vector<int>({30, 40, 50}), "khoang giua cay 25..55");
    kq.clear(); t.rangeSearch(10, 70, kq);
    kiemTra(kq.size() == 7, "khoang bao tron, tinh ca hai dau mut");
    kq.clear(); t.rangeSearch(30, 30, kq);
    kiemTra(kq == std::vector<int>({30}), "lo == hi, dung mot phan tu");
    kq.clear(); t.rangeSearch(31, 39, kq);
    kiemTra(kq.empty(), "khoang nam giua hai khoa, rong");
    kq.clear(); t.rangeSearch(100, 200, kq);
    kiemTra(kq.empty(), "khoang nam ngoai cay");
    kq.clear(); t.rangeSearch(50, 20, kq);
    kiemTra(kq.empty(), "lo > hi, khoang rong");
    { Cay rong; kq.clear(); rong.rangeSearch(0, 10, kq); kiemTra(kq.empty(), "cay rong"); }

    std::mt19937 rng(2026);
    Cay cay; std::map<int, int> ref;
    for (int i = 0; i < 1000; ++i) { int k = static_cast<int>(rng() % 5000); if (cay.insert(k, k * 3)) ref[k] = k * 3; }
    bool ok = true;
    for (int i = 0; i < 500; ++i) {
        int lo = static_cast<int>(rng() % 5200) - 100, hi = static_cast<int>(rng() % 5200) - 100;
        std::vector<int> got, want;
        cay.rangeSearch(lo, hi, got);
        if (lo <= hi) for (auto it = ref.lower_bound(lo); it != ref.end() && it->first <= hi; ++it) want.push_back(it->second);
        ok = ok && got == want;
    }
    kiemTra(ok, "500 khoang ngau nhien tren 1000 khoa, doi chieu std::map");
    for (int k = 0; k < 5000; k += 2) { cay.remove(k); ref.erase(k); }
    std::vector<int> got, want;
    cay.rangeSearch(0, 4999, got);
    for (auto& kv : ref) want.push_back(kv.second);
    kiemTra(got == want, "Range Search dung sau khi xoa mot nua so khoa");
}

// ============================================================ SỐ LIỆU CHO BÁO CÁO
static double ms(std::chrono::steady_clock::time_point a) {
    return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - a).count();
}
static void soLieu() {
    ghi("\nBANG DO CHIEU CAO VA THOI GIAN (khoa ngau nhien)\n");
    ghi("%-8s %-10s %-14s %-12s %-12s %-14s\n", "n", "chieu cao", "1.44*log2(n+2)", "them (ms)", "xoa (ms)", "100 range (ms)");
    std::mt19937 rng(2026);
    for (int n : {1000, 10000, 100000}) {
        std::vector<int> khoa(n);
        for (int i = 0; i < n; ++i) khoa[i] = i * 3;          // khóa phân biệt
        std::shuffle(khoa.begin(), khoa.end(), rng);
        Cay t;
        auto t0 = std::chrono::steady_clock::now();
        for (int k : khoa) t.insert(k, k);
        double msThem = ms(t0);
        int cao = t.height();
        t0 = std::chrono::steady_clock::now();
        for (int i = 0; i < 100; ++i) {
            std::vector<int> r; int lo = static_cast<int>(rng() % (3 * n)); t.rangeSearch(lo, lo + 3 * 20, r);
        }
        double msRange = ms(t0);
        std::shuffle(khoa.begin(), khoa.end(), rng);
        t0 = std::chrono::steady_clock::now();
        for (int k : khoa) t.remove(k);
        double msXoa = ms(t0);
        ghi("%-8d %-10d %-14.1f %-12.2f %-12.2f %-14.3f\n", n, cao, 1.44 * std::log2(n + 2.0), msThem, msXoa, msRange);
    }
}

int main(int argc, char** argv) {
    const char* tenFile = argc > 1 ? argv[1] : "ket_qua_test.txt";
    testQuay();
    testXoa();
    testRange();

    int tongDat = 0, tongTruot = 0;
    ghi("KET QUA BO TEST CAY AVL (T23)\n");
    ghi("%-16s %8s %8s %8s\n", "Nhom", "Dat", "Truot", "Tong");
    for (auto& g : g_nhom) {
        ghi("%-16s %8d %8d %8d\n", g.ten.c_str(), g.dat, g.truot, g.dat + g.truot);
        tongDat += g.dat; tongTruot += g.truot;
    }
    ghi("%-16s %8d %8d %8d\n", "TONG", tongDat, tongTruot, tongDat + tongTruot);
    for (auto& l : g_loi) ghi("LOI: %s\n", l.c_str());
    soLieu();
    ghi("\n%s\n", tongTruot == 0 ? "TAT CA TEST DAT" : "CO TEST TRUOT");

    if (FILE* f = std::fopen(tenFile, "w")) { std::fputs(g_dong.c_str(), f); std::fclose(f); std::printf("(Da ghi ket qua vao %s)\n", tenFile); }
    return tongTruot == 0 ? 0 : 1;
}
