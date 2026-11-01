#ifndef CHUONGTRINHSINHVADODULIEU_H
#define CHUONGTRINHSINHVADODULIEU_H

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <algorithm>
#include <iomanip>
#include "ngay.h"
#include "mathang.h"
#include "AVLTree.h"

inline std::string taoMaKhoaNgauNhien(int id) {
    std::string s = std::to_string(id);
    if (s.length() < 6)
        return "SP" + std::string(6 - s.length(), '0') + s;
    return "SP" + s;
}

// Benchmark tìm kiếm: AVL so với vector đã sắp xếp + lower_bound.
// Phép đo cập nhật gồm 100 lần chèn/xóa trên cùng dữ liệu và cùng dãy khóa.
inline volatile unsigned long long& benchmarkChecksumRef() {
    static volatile unsigned long long checksum = 0;
    return checksum;
}

inline long long trungVi(std::vector<long long> mau) {
    std::sort(mau.begin(), mau.end());
    return mau[mau.size() / 2];
}

typedef std::pair<std::string, MatHang> CapMatHang;

inline std::vector<CapMatHang>::iterator timVectorDaSapXep(
        std::vector<CapMatHang>& ds, const std::string& ma) {
    std::vector<CapMatHang>::iterator it = std::lower_bound(
        ds.begin(), ds.end(), ma,
        [](const CapMatHang& item, const std::string& key) { return item.first < key; });
    return (it != ds.end() && it->first == ma) ? it : ds.end();
}

inline std::vector<CapMatHang>::const_iterator timVectorDaSapXep(
        const std::vector<CapMatHang>& ds, const std::string& ma) {
    std::vector<CapMatHang>::const_iterator it = std::lower_bound(
        ds.begin(), ds.end(), ma,
        [](const CapMatHang& item, const std::string& key) { return item.first < key; });
    return (it != ds.end() && it->first == ma) ? it : ds.end();
}

inline void thucHienDoBenchmark(int quyMo) {
    using std::string; using std::vector; using std::cout; using std::endl;
    using std::make_pair; using std::to_string; using std::mt19937;
    using std::sort; using std::lower_bound; using std::fixed; using std::setprecision;
    using std::chrono::steady_clock; using std::chrono::duration_cast;
    using std::chrono::nanoseconds;

    const int SO_TRUY_VAN = 1000;
    const int SO_LAN_DO = 9;
    const int SO_CAP_NHAT = 100;

    cout << "\n==================================================\n";
    cout << "  QUY MO KIEM THU: " << quyMo << " MAT HANG\n";
    cout << "==================================================\n";

    AVLTree<string, MatHang> cayAVL;
    vector<CapMatHang> dsVector;
    dsVector.reserve(quyMo + SO_CAP_NHAT);

    // Dùng khóa chẵn làm dữ liệu gốc; khóa lẻ là các khóa chèn xen kẽ vào vector.
    for (int i = 1; i <= quyMo; ++i) {
        string maKhoa = taoMaKhoaNgauNhien(i * 2);
        string tenSp = "Hang_" + to_string(i);
        long long gia = 10000 + (i % 5000);
        int slTon = 100 + (i % 50);
        MatHang mh(maKhoa.c_str(), tenSp.c_str(), gia, slTon, 10, Ngay(24, 10, 2026));
        cayAVL.insert(maKhoa, mh);
        dsVector.push_back(make_pair(maKhoa, mh));
    }

    vector<string> dsTruyVan;
    mt19937 rng(2026);
    for (int i = 0; i < SO_TRUY_VAN; ++i) {
        int idQuery = ((rng() % quyMo) + 1) * 2;
        dsTruyVan.push_back(taoMaKhoaNgauNhien(idQuery));
    }

    vector<string> dsThem, dsXoa;
    vector<MatHang> matHangThem;
    for (int i = 1; i <= SO_CAP_NHAT; ++i) {
        string ma = taoMaKhoaNgauNhien(i * 2 - 1);
        dsThem.push_back(ma);
        string ten = "Hang_chen_" + to_string(i);
        matHangThem.push_back(MatHang(ma.c_str(), ten.c_str(), 15000 + i, 100, 10, Ngay(24, 10, 2026)));
        int index = (i * (quyMo / SO_CAP_NHAT));
        if (index < 1) index = 1;
        if (index > quyMo) index = quyMo;
        dsXoa.push_back(taoMaKhoaNgauNhien(index * 2));
    }

    // Warm-up cả hai phương pháp bằng cùng tập truy vấn.
    unsigned long long warmupChecksum = 0;
    for (int i = 0; i < SO_TRUY_VAN; ++i) {
        MatHang* kq = cayAVL.search(dsTruyVan[i]);
        if (kq) warmupChecksum += static_cast<unsigned long long>(kq->donGia);
        vector<CapMatHang>::const_iterator it = timVectorDaSapXep(dsVector, dsTruyVan[i]);
        if (it != dsVector.end()) warmupChecksum += static_cast<unsigned long long>(it->second.donGia);
    }
    benchmarkChecksumRef() += warmupChecksum;

    vector<long long> mauAVL, mauVector;
    int demTimThayAVL = 0, demTimThayVec = 0;
    for (int lan = 0; lan < SO_LAN_DO; ++lan) {
        int hitAVL = 0;
        unsigned long long checksumAVL = 0;
        steady_clock::time_point startAVL = steady_clock::now();
        for (size_t i = 0; i < dsTruyVan.size(); ++i) {
            MatHang* kq = cayAVL.search(dsTruyVan[i]);
            if (kq) { ++hitAVL; checksumAVL += static_cast<unsigned long long>(kq->donGia); }
        }
        steady_clock::time_point stopAVL = steady_clock::now();
        mauAVL.push_back(duration_cast<nanoseconds>(stopAVL - startAVL).count());
        benchmarkChecksumRef() += checksumAVL;
        demTimThayAVL = hitAVL;

        int hitVec = 0;
        unsigned long long checksumVec = 0;
        steady_clock::time_point startVec = steady_clock::now();
        for (size_t i = 0; i < dsTruyVan.size(); ++i) {
            vector<CapMatHang>::const_iterator it = timVectorDaSapXep(dsVector, dsTruyVan[i]);
            if (it != dsVector.end()) { ++hitVec; checksumVec += static_cast<unsigned long long>(it->second.donGia); }
        }
        steady_clock::time_point stopVec = steady_clock::now();
        mauVector.push_back(duration_cast<nanoseconds>(stopVec - startVec).count());
        benchmarkChecksumRef() += checksumVec;
        demTimThayVec = hitVec;
    }

    // Chuẩn bị cấu trúc ngoài vùng đo; chỉ tính 100 thao tác nghiệp vụ.
    vector<long long> mauThemAVL, mauThemVector, mauXoaAVL, mauXoaVector;
    for (int lan = 0; lan < SO_LAN_DO; ++lan) {
        AVLTree<string, MatHang> cayThem;
        for (size_t i = 0; i < dsVector.size(); ++i) cayThem.insert(dsVector[i].first, dsVector[i].second);
        steady_clock::time_point t0 = steady_clock::now();
        for (int i = 0; i < SO_CAP_NHAT; ++i) cayThem.insert(dsThem[i], matHangThem[i]);
        steady_clock::time_point t1 = steady_clock::now();
        mauThemAVL.push_back(duration_cast<nanoseconds>(t1 - t0).count());

        vector<CapMatHang> vecThem(dsVector);
        t0 = steady_clock::now();
        for (int i = 0; i < SO_CAP_NHAT; ++i) {
            vector<CapMatHang>::iterator pos = lower_bound(
                vecThem.begin(), vecThem.end(), dsThem[i],
                [](const CapMatHang& item, const string& key) { return item.first < key; });
            vecThem.insert(pos, make_pair(dsThem[i], matHangThem[i]));
        }
        t1 = steady_clock::now();
        mauThemVector.push_back(duration_cast<nanoseconds>(t1 - t0).count());

        AVLTree<string, MatHang> cayXoa;
        for (size_t i = 0; i < dsVector.size(); ++i) cayXoa.insert(dsVector[i].first, dsVector[i].second);
        t0 = steady_clock::now();
        for (int i = 0; i < SO_CAP_NHAT; ++i) cayXoa.remove(dsXoa[i]);
        t1 = steady_clock::now();
        mauXoaAVL.push_back(duration_cast<nanoseconds>(t1 - t0).count());

        vector<CapMatHang> vecXoa(dsVector);
        t0 = steady_clock::now();
        for (int i = 0; i < SO_CAP_NHAT; ++i) {
            vector<CapMatHang>::iterator pos = timVectorDaSapXep(vecXoa, dsXoa[i]);
            if (pos != vecXoa.end()) vecXoa.erase(pos);
        }
        t1 = steady_clock::now();
        mauXoaVector.push_back(duration_cast<nanoseconds>(t1 - t0).count());
    }

    const double avlUs = trungVi(mauAVL) / 1000.0;
    const double vecUs = trungVi(mauVector) / 1000.0;
    const double themAVLUs = trungVi(mauThemAVL) / 1000.0;
    const double themVecUs = trungVi(mauThemVector) / 1000.0;
    const double xoaAVLUs = trungVi(mauXoaAVL) / 1000.0;
    const double xoaVecUs = trungVi(mauXoaVector) / 1000.0;

    cout << " - Chieu cao cay AVL (" << cayAVL.size() << " nut): " << cayAVL.height() << "\n";
    cout << " - So truy van khop: " << demTimThayAVL << "/" << demTimThayVec
         << " (" << SO_TRUY_VAN << " truy van; " << SO_LAN_DO << " lan do)\n";
    cout << fixed << setprecision(3);
    cout << " - Trung vi AVL:     " << avlUs << " microseconds / " << SO_TRUY_VAN << " truy van\n";
    cout << " - Trung vi Vector (sorted + lower_bound): " << vecUs << " microseconds / " << SO_TRUY_VAN << " truy van\n";
    if (avlUs > 0.0) cout << " ==> Vector/AVL (tim kiem): " << setprecision(2) << (vecUs / avlUs) << " lan\n";
    cout << setprecision(3);
    cout << " - Them " << SO_CAP_NHAT << " khoa | AVL: " << themAVLUs << " us | Vector sorted: " << themVecUs << " us\n";
    cout << " - Xoa  " << SO_CAP_NHAT << " khoa | AVL: " << xoaAVLUs << " us | Vector sorted: " << xoaVecUs << " us\n";
}

inline void chayToanBoBenchmark() {
    std::cout << "\n==================================================\n";
    std::cout << "   CHUONG TRINH BENCHMARK HIEU NANG AVL\n";
    std::cout << "==================================================\n";

    thucHienDoBenchmark(1000);
    thucHienDoBenchmark(10000);
    thucHienDoBenchmark(50000);
    thucHienDoBenchmark(100000);

    std::cout << "\n==================================================\n";
    std::cout << "Kiem thu hieu nang hoan tat thanh cong!\n";
}

#endif // CHUONGTRINHSINHVADODULIEU_H
