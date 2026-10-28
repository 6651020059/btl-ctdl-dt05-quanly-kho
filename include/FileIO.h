// FileIO.h - T45 (ghi) + T46 (doc) - TV5
// Dinh dang file nhi phan (ban ghi co do dai co dinh):
//   [FileHeader][MatHang x soMatHang][HoaDon x soHoaDon]
#ifndef FILEIO_H
#define FILEIO_H

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include "mathang.h"
#include "hoadon.h"

namespace FileIO {

struct FileHeader {
    char     magic[4];       // "SMK1"
    uint32_t version;        // 1
    uint32_t kichThuocMH;    // sizeof(MatHang)  - phat hien file tao boi ban build khac
    uint32_t kichThuocHD;    // sizeof(HoaDon)
    uint32_t soMatHang;
    uint32_t soHoaDon;
};

const char     MAGIC[4] = {'S', 'M', 'K', '1'};
const uint32_t VERSION  = 1;

struct FileCloser { void operator()(FILE* f) const { if (f) std::fclose(f); } };
typedef std::unique_ptr<FILE, FileCloser> FilePtr;

// ---------- T45: GHI FILE ----------
// Ghi ra file tam roi doi ten: neu loi giua chung thi file cu van nguyen ven.
inline bool ghiFile(const std::string& path,
                    const std::vector<MatHang>& dsMH,
                    const std::vector<HoaDon>& dsHD,
                    std::string& loi) {
    const std::string tmp = path + ".tmp";
    bool ok = true;
    {
        FilePtr f(std::fopen(tmp.c_str(), "wb"));
        if (!f) { loi = "Khong mo duoc file de ghi: " + tmp; return false; }

        FileHeader h;
        std::memset(&h, 0, sizeof h);
        std::memcpy(h.magic, MAGIC, 4);
        h.version     = VERSION;
        h.kichThuocMH = (uint32_t)sizeof(MatHang);
        h.kichThuocHD = (uint32_t)sizeof(HoaDon);
        h.soMatHang   = (uint32_t)dsMH.size();
        h.soHoaDon    = (uint32_t)dsHD.size();

        ok = std::fwrite(&h, sizeof h, 1, f.get()) == 1;
        if (ok && !dsMH.empty())
            ok = std::fwrite(dsMH.data(), sizeof(MatHang), dsMH.size(), f.get()) == dsMH.size();
        if (ok && !dsHD.empty())
            ok = std::fwrite(dsHD.data(), sizeof(HoaDon), dsHD.size(), f.get()) == dsHD.size();
        if (std::fflush(f.get()) != 0) ok = false;
    }
    if (!ok) { std::remove(tmp.c_str()); loi = "Loi khi ghi du lieu (day o dia?)"; return false; }

    std::remove(path.c_str());
    if (std::rename(tmp.c_str(), path.c_str()) != 0) {
        loi = "Khong doi ten duoc file tam sang " + path;
        return false;
    }
    return true;
}

// ---------- T46: DOC FILE ----------
inline bool docFile(const std::string& path,
                    std::vector<MatHang>& dsMH,
                    std::vector<HoaDon>& dsHD,
                    std::string& loi) {
    dsMH.clear();
    dsHD.clear();

    FilePtr f(std::fopen(path.c_str(), "rb"));
    if (!f) { loi = "File khong ton tai: " + path; return false; }

    std::fseek(f.get(), 0, SEEK_END);
    long size = std::ftell(f.get());
    std::fseek(f.get(), 0, SEEK_SET);
    if (size < (long)sizeof(FileHeader)) { loi = "File qua ngan, khong co header"; return false; }

    FileHeader h;
    if (std::fread(&h, sizeof h, 1, f.get()) != 1) { loi = "Khong doc duoc header"; return false; }
    if (std::memcmp(h.magic, MAGIC, 4) != 0)       { loi = "Sai dinh dang (magic khong khop)"; return false; }
    if (h.version != VERSION)                      { loi = "Phien ban file khong ho tro"; return false; }
    if (h.kichThuocMH != sizeof(MatHang) || h.kichThuocHD != sizeof(HoaDon)) {
        loi = "File duoc tao boi ban build khac (kich thuoc ban ghi khong khop)";
        return false;
    }

    // Kich thuoc file phai KHOP CHINH XAC voi so ban ghi trong header
    uint64_t can = sizeof(FileHeader)
                 + (uint64_t)h.soMatHang * sizeof(MatHang)
                 + (uint64_t)h.soHoaDon  * sizeof(HoaDon);
    if (can != (uint64_t)size) { loi = "File hong: kich thuoc khong khop so ban ghi"; return false; }

    dsMH.resize(h.soMatHang);
    if (h.soMatHang &&
        std::fread(dsMH.data(), sizeof(MatHang), h.soMatHang, f.get()) != h.soMatHang) {
        dsMH.clear(); loi = "File hong: thieu du lieu mat hang"; return false;
    }
    dsHD.resize(h.soHoaDon);
    if (h.soHoaDon &&
        std::fread(dsHD.data(), sizeof(HoaDon), h.soHoaDon, f.get()) != h.soHoaDon) {
        dsMH.clear(); dsHD.clear(); loi = "File hong: thieu du lieu hoa don"; return false;
    }

    // Kiem tra noi dung hop le
    for (size_t i = 0; i < dsMH.size(); ++i) {
        MatHang& m = dsMH[i];
        m.maHang[sizeof m.maHang - 1]   = '\0';
        m.tenHang[sizeof m.tenHang - 1] = '\0';
        if (m.maHang[0] == '\0' || m.soLuongTon < 0 || m.donGia < 0 || m.tonToiThieu < 0) {
            dsMH.clear(); dsHD.clear();
            loi = "File hong: mat hang thu " + std::to_string(i + 1) + " co du lieu khong hop le";
            return false;
        }
    }
    for (size_t i = 0; i < dsHD.size(); ++i) {
        HoaDon& d = dsHD[i];
        d.maHoaDon[sizeof d.maHoaDon - 1] = '\0';
        if (d.soLuongChiTiet < 0 || d.soLuongChiTiet > MAX_CHI_TIET) {
            dsMH.clear(); dsHD.clear();
            loi = "File hong: hoa don thu " + std::to_string(i + 1) + " co so chi tiet vo ly";
            return false;
        }
        for (int k = 0; k < d.soLuongChiTiet; ++k)
            d.danhSachChiTiet[k].maHang[sizeof d.danhSachChiTiet[k].maHang - 1] = '\0';
    }
    return true;
}

// ---------- Cau noi voi cay (KhoHang = AVLTree<std::string, MatHang>) ----------
// Chi can cay co: inorder() tra ve vector<MatHang>, clear(), insert(key, value).
// Nen chay duoc voi AVLTree_Stub.h bay gio va AVLTree.h that sau nay, khong phai sua.

// Luu toan bo kho + danh sach hoa don ra file.
template <typename Kho>
inline bool luuDuLieu(const std::string& path, const Kho& kho,
                      const std::vector<HoaDon>& dsHD, std::string& loi) {
    return ghiFile(path, kho.inorder(), dsHD, loi);
}

// Doc file va nap vao kho. Neu loi thi kho va dsHD KHONG bi thay doi.
template <typename Kho>
inline bool napDuLieu(const std::string& path, Kho& kho,
                      std::vector<HoaDon>& dsHD, std::string& loi) {
    std::vector<MatHang> mh;
    std::vector<HoaDon>  hd;
    if (!docFile(path, mh, hd, loi)) return false;

    std::set<std::string> daCo;                 // phat hien ma hang trung (file hong)
    for (size_t i = 0; i < mh.size(); ++i)
        if (!daCo.insert(std::string(mh[i].maHang)).second) {
            loi = std::string("File hong: trung ma hang ") + mh[i].maHang;
            return false;
        }

    kho.clear();
    for (size_t i = 0; i < mh.size(); ++i)
        kho.insert(std::string(mh[i].maHang), mh[i]);
    dsHD = hd;
    return true;
}

} // namespace FileIO

#endif // FILEIO_H
