#ifndef CHUONGTRINHSINHVADODULIEU_H
#define CHUONGTRINHSINHVADODULIEU_H

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <algorithm>
#include "ngay.h"
#include "mathang.h"
#include "AVLTree.h"

inline std::string taoMaKhoaNgauNhien(int id) {
    std::string s = std::to_string(id);
    if (s.length() < 5)
        return "SP" + std::string(5 - s.length(), '0') + s;
    return "SP" + s;
}

inline void thucHienDoBenchmark(int quyMo) {
    using namespace std;
    using namespace std::chrono;

    cout << "\n==================================================\n";
    cout << "  QUY MÔ KIỂM THỬ: " << quyMo << " MẶT HÀNG\n";
    cout << "==================================================\n";

    AVLTree<string, MatHang> cayAVL;
    vector<pair<string, MatHang>> dsVector;
    dsVector.reserve(quyMo);

    for (int i = 1; i <= quyMo; ++i) {
        string maKhoa = taoMaKhoaNgauNhien(i);
        string tenSp = "Hang_" + to_string(i);

        long long gia = 10000 + (i % 5000);
        int slTon = 100 + (i % 50);
        int tonMin = 10;
        Ngay ngayNhapHang(24, 10, 2026);

        MatHang mh(maKhoa.c_str(), tenSp.c_str(), gia, slTon, tonMin, ngayNhapHang);
        cayAVL.insert(maKhoa, mh);
        dsVector.push_back(make_pair(maKhoa, mh));
    }

    vector<string> dsTruyVan;
    mt19937 rng(2026);
    for (int i = 0; i < 1000; ++i) {
        int idQuery = (rng() % quyMo) + 1;
        dsTruyVan.push_back(taoMaKhoaNgauNhien(idQuery));
    }

    auto startAVL = high_resolution_clock::now();
    int demTimThayAVL = 0;
    for (size_t i = 0; i < dsTruyVan.size(); ++i) {
        MatHang* kq = cayAVL.search(dsTruyVan[i]);
        if (kq != nullptr) {
            demTimThayAVL++;
        }
    }
    auto stopAVL = high_resolution_clock::now();
    auto durationAVL = duration_cast<microseconds>(stopAVL - startAVL).count();

    auto startVec = high_resolution_clock::now();
    int demTimThayVec = 0;
    for (size_t i = 0; i < dsTruyVan.size(); ++i) {
        const string& maCanTim = dsTruyVan[i];
        for (size_t j = 0; j < dsVector.size(); ++j) {
            if (dsVector[j].first == maCanTim) {
                demTimThayVec++;
                break;
            }
        }
    }
    auto stopVec = high_resolution_clock::now();
    auto durationVec = duration_cast<microseconds>(stopVec - startVec).count();

    cout << " - Chiều cao cây AVL (" << cayAVL.size() << " nút): " << cayAVL.height() << "\n";
    cout << " - Số truy vấn khớp: " << demTimThayAVL << "/" << demTimThayVec << " (1.000 truy vấn)\n";
    cout << " - Thời gian tìm kiếm trên Cây AVL:     " << durationAVL << " microseconds\n";
    cout << " - Thời gian tìm kiếm Tuyến tính Vector: " << durationVec << " microseconds\n";

    if (durationAVL > 0) {
        double tiLeNhanh = (double)durationVec / durationAVL;
        cout << " ==> Cây AVL nhanh hơn Vector khoảng: " << tiLeNhanh << " lần!\n";
    }
}

inline void chayToanBoBenchmark() {
    std::cout << "\n==================================================\n";
    std::cout << "   CHƯƠNG TRÌNH BENCHMARK HIỆU NĂNG AVL (TASK 40)\n";
    std::cout << "==================================================\n";

    thucHienDoBenchmark(1000);
    thucHienDoBenchmark(10000);
    thucHienDoBenchmark(50000);

    std::cout << "\n==================================================\n";
    std::cout << "Kiểm thử hiệu năng Task 40 hoàn tất thành công!\n";
}

#endif // CHUONGTRINHSINHVADODULIEU_H
