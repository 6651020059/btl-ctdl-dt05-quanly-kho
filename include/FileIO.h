// FileIO.h - T45 (ghi) + T46 (doc) - TV5
// Dinh dang file nhi phan (ban ghi co do dai co dinh):
//   [FileHeader][MatHang x soMatHang][HoaDon x soHoaDon]
#ifndef FILEIO_H
#define FILEIO_H

#include <cstdint>
#include <cstdio>
#include <cstddef>
#include <climits>
#include <limits>
#include <cstring>
#include <memory>
#include <set>
#include <string>
#include <vector>
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif
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


inline bool hopLeMatHang(const MatHang& m) {
    return m.maHang[0] != '\0' && m.tenHang[0] != '\0' &&
           m.donGia >= 0 && m.soLuongTon >= 0 && m.tonToiThieu >= 0 &&
           ngayHopLe(m.ngayNhap, 1, 9999);
}

inline bool hopLeHoaDon(const HoaDon& d, std::string& loiChiTiet) {
    if (d.maHoaDon[0] == '\0' || !ngayHopLe(d.ngayLap, 1, 9999) ||
        d.soLuongChiTiet < 1 || d.soLuongChiTiet > MAX_CHI_TIET || d.tongTien < 0) {
        loiChiTiet = "thong tin chung hoac so chi tiet khong hop le";
        return false;
    }
    long long tong = 0;
    for (int k = 0; k < d.soLuongChiTiet; ++k) {
        const ChiTietHoaDon& ct = d.danhSachChiTiet[k];
        if (ct.maHang[0] == '\0' || ct.soLuongMua <= 0 || ct.donGia < 0 ||
            (ct.donGia > 0 && ct.soLuongMua > LLONG_MAX / ct.donGia)) {
            loiChiTiet = "dong chi tiet " + std::to_string(k + 1) + " khong hop le";
            return false;
        }
        const long long thanhTien = static_cast<long long>(ct.soLuongMua) * ct.donGia;
        if (thanhTien > LLONG_MAX - tong) {
            loiChiTiet = "tong tien dong chi tiet bi tran";
            return false;
        }
        tong += thanhTien;
    }
    if (tong != d.tongTien) {
        loiChiTiet = "tong tien khong khop cac dong chi tiet";
        return false;
    }
    return true;
}

// Serialize theo layout ABI hiện tại nhưng khởi tạo byte đệm về 0 trước khi chép trường.
inline void maHoaMatHang(const MatHang& src, unsigned char* dst) {
    std::memset(dst, 0, sizeof(MatHang));
#define COPY_MH_FIELD(field) std::memcpy(dst + offsetof(MatHang, field), &src.field, sizeof(src.field))
    COPY_MH_FIELD(maHang); COPY_MH_FIELD(tenHang); COPY_MH_FIELD(donGia);
    COPY_MH_FIELD(soLuongTon); COPY_MH_FIELD(tonToiThieu);
    std::memcpy(dst + offsetof(MatHang, ngayNhap) + offsetof(Ngay, ngay), &src.ngayNhap.ngay, sizeof(src.ngayNhap.ngay));
    std::memcpy(dst + offsetof(MatHang, ngayNhap) + offsetof(Ngay, thang), &src.ngayNhap.thang, sizeof(src.ngayNhap.thang));
    std::memcpy(dst + offsetof(MatHang, ngayNhap) + offsetof(Ngay, nam), &src.ngayNhap.nam, sizeof(src.ngayNhap.nam));
#undef COPY_MH_FIELD
}

inline void maHoaHoaDon(const HoaDon& src, unsigned char* dst) {
    std::memset(dst, 0, sizeof(HoaDon));
    std::memcpy(dst + offsetof(HoaDon, maHoaDon), src.maHoaDon, sizeof(src.maHoaDon));
    std::memcpy(dst + offsetof(HoaDon, ngayLap) + offsetof(Ngay, ngay), &src.ngayLap.ngay, sizeof(src.ngayLap.ngay));
    std::memcpy(dst + offsetof(HoaDon, ngayLap) + offsetof(Ngay, thang), &src.ngayLap.thang, sizeof(src.ngayLap.thang));
    std::memcpy(dst + offsetof(HoaDon, ngayLap) + offsetof(Ngay, nam), &src.ngayLap.nam, sizeof(src.ngayLap.nam));
    std::memcpy(dst + offsetof(HoaDon, soLuongChiTiet), &src.soLuongChiTiet, sizeof(src.soLuongChiTiet));
    std::memcpy(dst + offsetof(HoaDon, tongTien), &src.tongTien, sizeof(src.tongTien));
    for (int k = 0; k < src.soLuongChiTiet && k < MAX_CHI_TIET; ++k) {
        const size_t base = offsetof(HoaDon, danhSachChiTiet) + static_cast<size_t>(k) * sizeof(ChiTietHoaDon);
        const ChiTietHoaDon& ct = src.danhSachChiTiet[k];
        std::memcpy(dst + base + offsetof(ChiTietHoaDon, maHang), ct.maHang, sizeof(ct.maHang));
        std::memcpy(dst + base + offsetof(ChiTietHoaDon, soLuongMua), &ct.soLuongMua, sizeof(ct.soLuongMua));
        std::memcpy(dst + base + offsetof(ChiTietHoaDon, donGia), &ct.donGia, sizeof(ct.donGia));
    }
}

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
        std::set<std::string> maHangDaCo;
        for (size_t i = 0; ok && i < dsMH.size(); ++i) {
            if (!hopLeMatHang(dsMH[i]) || !maHangDaCo.insert(std::string(dsMH[i].maHang)).second) {
                loi = "Du lieu mat hang khong hop le hoac trung ma: " + std::string(dsMH[i].maHang);
                ok = false;
                break;
            }
            std::vector<unsigned char> record(sizeof(MatHang), 0);
            maHoaMatHang(dsMH[i], record.data());
            ok = std::fwrite(record.data(), 1, record.size(), f.get()) == record.size();
        }
        std::set<std::string> maHoaDonDaCo;
        for (size_t i = 0; ok && i < dsHD.size(); ++i) {
            std::string loiHD;
            if (!hopLeHoaDon(dsHD[i], loiHD) ||
                !maHoaDonDaCo.insert(std::string(dsHD[i].maHoaDon)).second) {
                loi = "Du lieu hoa don khong hop le hoac trung ma: " + std::string(dsHD[i].maHoaDon) + " (" + loiHD + ")";
                ok = false;
                break;
            }
            std::vector<unsigned char> record(sizeof(HoaDon), 0);
            maHoaHoaDon(dsHD[i], record.data());
            ok = std::fwrite(record.data(), 1, record.size(), f.get()) == record.size();
        }
        if (std::fflush(f.get()) != 0) ok = false;
    }
    if (!ok) { std::remove(tmp.c_str()); if (loi.empty()) loi = "Loi khi ghi du lieu (day o dia?)"; return false; }

#ifdef _WIN32
    // Windows rename() khong ghi de tep dich da ton tai; MoveFileEx thay the tep.
    if (!MoveFileExA(tmp.c_str(), path.c_str(),
                     MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) {
        std::remove(tmp.c_str());
        loi = "Khong thay the duoc file dich (MoveFileExA that bai)";
        return false;
    }
#else
    // POSIX rename tren cung filesystem thay the tep dich theo cach nguyen tu.
    if (std::rename(tmp.c_str(), path.c_str()) != 0) {
        std::remove(tmp.c_str());
        loi = "Khong doi ten duoc file tam sang " + path;
        return false;
    }
#endif
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

    // Kiem tra noi dung tung ban ghi, bao gom ngay va cac dong chi tiet hoa don.
    std::set<std::string> maHangDaCo;
    for (size_t i = 0; i < dsMH.size(); ++i) {
        MatHang& m = dsMH[i];
        m.maHang[sizeof m.maHang - 1] = '\0';
        m.tenHang[sizeof m.tenHang - 1] = '\0';
        if (!hopLeMatHang(m) || !maHangDaCo.insert(std::string(m.maHang)).second) {
            dsMH.clear(); dsHD.clear();
            loi = "File hong: mat hang thu " + std::to_string(i + 1) + " co du lieu khong hop le hoac trung ma";
            return false;
        }
    }
    std::set<std::string> maHoaDonDaCo;
    for (size_t i = 0; i < dsHD.size(); ++i) {
        HoaDon& d = dsHD[i];
        d.maHoaDon[sizeof d.maHoaDon - 1] = '\0';
        for (int k = 0; k < MAX_CHI_TIET; ++k)
            d.danhSachChiTiet[k].maHang[sizeof d.danhSachChiTiet[k].maHang - 1] = '\0';
        std::string loiHD;
        if (!hopLeHoaDon(d, loiHD) || !maHoaDonDaCo.insert(std::string(d.maHoaDon)).second) {
            dsMH.clear(); dsHD.clear();
            loi = "File hong: hoa don thu " + std::to_string(i + 1) + " khong hop le: " + loiHD;
            return false;
        }
    }
    return true;
}

// ---------- Cau noi voi cay (KhoHang = AVLTree<std::string, MatHang>) ----------
// Chi can cay co: inorder() tra ve vector<MatHang>, clear(), insert(key, value).
// Chay voi AVLTree.h that (AVLTree_Stub.h cu da bo khoi repo).

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
