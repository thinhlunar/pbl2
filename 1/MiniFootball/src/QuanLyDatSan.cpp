#include "QuanLyDatSan.h"
#include "BoDatSan.h"

#include <algorithm>
#include <iostream>

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
QuanLyDatSan::QuanLyDatSan(QuanLySan& quanLySan, QuanLyKhachHang& quanLyKhachHang)
    : quanLySan(quanLySan), quanLyKhachHang(quanLyKhachHang)
{
}

// ---------------------------------------------------------------------------
// Private: raw overlap check (no validation — callers must validate first)
// ---------------------------------------------------------------------------
bool QuanLyDatSan::coTrungLich(int maSan,
                                const std::string& ngay,
                                const std::string& gioBatDau,
                                const std::string& gioKetThuc,
                                int ngoaiTruId) const
{
    int s1 = BoDatSan::phanTichGio(gioBatDau);
    int e1 = BoDatSan::phanTichGio(gioKetThuc);

    for (const DatSan& ds : danhSachDatSan)
    {
        if (ds.getTrangThai() == "CANCELLED")
            continue;
        if (ngoaiTruId >= 0 && ds.getId() == ngoaiTruId)
            continue;
        if (ds.getMaSan() != maSan || ds.getNgay() != ngay)
            continue;

        int s2 = BoDatSan::phanTichGio(ds.getGioBatDau());
        int e2 = BoDatSan::phanTichGio(ds.getGioKetThuc());

        // Overlap: s1 < e2 AND s2 < e1
        if (s1 < e2 && s2 < e1)
            return true;
    }
    return false;
}

// ---------------------------------------------------------------------------
// themDatSan — validated add
// ---------------------------------------------------------------------------
bool QuanLyDatSan::themDatSan(const DatSan& datSan)
{
    // 1. Date validation
    if (!BoDatSan::ngayHopLe(datSan.getNgay()))
        return false;

    // 2. Time interval validation
    if (!BoDatSan::khoangGioHopLe(datSan.getGioBatDau(), datSan.getGioKetThuc()))
        return false;

    // 3. Field existence + active status
    const SanBong* san = quanLySan.timTheoId(datSan.getMaSan());
    if (san == nullptr || !san->getTrangThai())
        return false;

    // 4. Customer existence
    if (!quanLyKhachHang.tonTai(datSan.getMaKhachHang()))
        return false;

    // 5. Overlap check
    if (coTrungLich(datSan.getMaSan(), datSan.getNgay(),
                    datSan.getGioBatDau(), datSan.getGioKetThuc(), -1))
        return false;

    danhSachDatSan.push_back(datSan);
    return true;
}

// ---------------------------------------------------------------------------
// huyDatSan
// ---------------------------------------------------------------------------
bool QuanLyDatSan::huyDatSan(int id)
{
    DatSan* ds = timTheoId(id);
    if (ds == nullptr)
        return false;
    ds->setTrangThai("CANCELLED");
    return true;
}

// ---------------------------------------------------------------------------
// suaDatSan — validated edit
// ---------------------------------------------------------------------------
bool QuanLyDatSan::suaDatSan(int id,
                               const std::string& ngay,
                               const std::string& gioBatDau,
                               const std::string& gioKetThuc)
{
    DatSan* ds = timTheoId(id);
    if (ds == nullptr)
        return false;

    // 1. Date validation
    if (!BoDatSan::ngayHopLe(ngay))
        return false;

    // 2. Time interval validation
    if (!BoDatSan::khoangGioHopLe(gioBatDau, gioKetThuc))
        return false;

    // 3. Overlap check — exclude this booking itself
    if (coTrungLich(ds->getMaSan(), ngay, gioBatDau, gioKetThuc, id))
        return false;

    ds->setNgay(ngay);
    ds->setGioBatDau(gioBatDau);
    ds->setGioKetThuc(gioKetThuc);
    return true;
}

// ---------------------------------------------------------------------------
// timTheoId
// ---------------------------------------------------------------------------
DatSan* QuanLyDatSan::timTheoId(int id)
{
    for (DatSan& ds : danhSachDatSan)
        if (ds.getId() == id)
            return &ds;
    return nullptr;
}

const DatSan* QuanLyDatSan::timTheoId(int id) const
{
    for (const DatSan& ds : danhSachDatSan)
        if (ds.getId() == id)
            return &ds;
    return nullptr;
}

// ---------------------------------------------------------------------------
// layDatSanTheoSan
// ---------------------------------------------------------------------------
std::vector<DatSan*> QuanLyDatSan::layDatSanTheoSan(int maSan)
{
    std::vector<DatSan*> result;
    for (DatSan& ds : danhSachDatSan)
        if (ds.getMaSan() == maSan)
            result.push_back(&ds);
    return result;
}

std::vector<const DatSan*> QuanLyDatSan::layDatSanTheoSan(int maSan) const
{
    std::vector<const DatSan*> result;
    for (const DatSan& ds : danhSachDatSan)
        if (ds.getMaSan() == maSan)
            result.push_back(&ds);
    return result;
}

// ---------------------------------------------------------------------------
// layDatSanTheoKhachHang
// ---------------------------------------------------------------------------
std::vector<DatSan*> QuanLyDatSan::layDatSanTheoKhachHang(int maKhachHang)
{
    std::vector<DatSan*> result;
    for (DatSan& ds : danhSachDatSan)
        if (ds.getMaKhachHang() == maKhachHang)
            result.push_back(&ds);
    return result;
}

std::vector<const DatSan*> QuanLyDatSan::layDatSanTheoKhachHang(int maKhachHang) const
{
    std::vector<const DatSan*> result;
    for (const DatSan& ds : danhSachDatSan)
        if (ds.getMaKhachHang() == maKhachHang)
            result.push_back(&ds);
    return result;
}

// ---------------------------------------------------------------------------
// hienThiDanhSach
// ---------------------------------------------------------------------------
void QuanLyDatSan::hienThiDanhSach() const
{
    std::cout << "=== DANH SACH DAT SAN ===\n";
    for (const DatSan& ds : danhSachDatSan)
    {
        std::cout << "ID: "            << ds.getId()
                  << " | San: "         << ds.getMaSan()
                  << " | KH: "          << ds.getMaKhachHang()
                  << " | Ngay: "        << ds.getNgay()
                  << " | "              << ds.getGioBatDau()
                  << " - "              << ds.getGioKetThuc()
                  << " | Trang thai: "  << ds.getTrangThai()
                  << "\n";
    }
}

// ---------------------------------------------------------------------------
// kiemTraTrungLich — public, no validation (callers validate externally)
// ---------------------------------------------------------------------------
bool QuanLyDatSan::kiemTraTrungLich(int maSan,
                                     const std::string& ngay,
                                     const std::string& gioBatDau,
                                     const std::string& gioKetThuc,
                                     int ngoaiTruId) const
{
    return coTrungLich(maSan, ngay, gioBatDau, gioKetThuc, ngoaiTruId);
}

// ---------------------------------------------------------------------------
// kiemTraSanTrong — full availability check (validates + checks overlap)
// ---------------------------------------------------------------------------
bool QuanLyDatSan::kiemTraSanTrong(int maSan,
                                    const std::string& ngay,
                                    const std::string& gioBatDau,
                                    const std::string& gioKetThuc,
                                    int ngoaiTruId) const
{
    // 1. Field exists and is active
    const SanBong* san = quanLySan.timTheoId(maSan);
    if (san == nullptr || !san->getTrangThai())
        return false;

    // 2. Date valid
    if (!BoDatSan::ngayHopLe(ngay))
        return false;

    // 3. Time interval valid
    if (!BoDatSan::khoangGioHopLe(gioBatDau, gioKetThuc))
        return false;

    // 4. No overlap
    return !coTrungLich(maSan, ngay, gioBatDau, gioKetThuc, ngoaiTruId);
}

// ---------------------------------------------------------------------------
// getDanhSach
// ---------------------------------------------------------------------------
const std::vector<DatSan>& QuanLyDatSan::getDanhSach() const
{
    return danhSachDatSan;
}

// ===========================================================================
// PRICING (Phase 5)
// ===========================================================================

namespace
{
    // Pricing period table (all times in minutes from midnight)
    struct KhoangGia
    {
        int    batDau; // inclusive
        int    ketThuc; // exclusive
        double giaPerHour;
    };

    // 00:00–06:00: 100,000   (early morning, same rate as daytime)
    // 06:00–16:00: 100,000   (daytime)
    // 16:00–22:00: 150,000   (peak evening)
    // 22:00–24:00: 100,000   (late night)
    static const KhoangGia BANG_GIA[] = {
        {    0,  360, 100000.0},
        {  360,  960, 100000.0},
        {  960, 1320, 150000.0},
        { 1320, 1440, 100000.0},
    };
    static const int SO_KHOANG = static_cast<int>(sizeof(BANG_GIA) / sizeof(BANG_GIA[0]));
} // anonymous namespace

// ---------------------------------------------------------------------------
// tinhTienTheoGio (static)
// ---------------------------------------------------------------------------
double QuanLyDatSan::tinhTienTheoGio(const std::string& gioBatDau,
                                      const std::string& gioKetThuc)
{
    if (!BoDatSan::khoangGioHopLe(gioBatDau, gioKetThuc))
        return -1.0;

    int start = BoDatSan::phanTichGio(gioBatDau); // minutes
    int end   = BoDatSan::phanTichGio(gioKetThuc);

    double tong = 0.0;
    for (int i = 0; i < SO_KHOANG; ++i)
    {
        int overlapStart = std::max(start, BANG_GIA[i].batDau);
        int overlapEnd   = std::min(end,   BANG_GIA[i].ketThuc);
        if (overlapEnd > overlapStart)
        {
            double soPhut = static_cast<double>(overlapEnd - overlapStart);
            tong += (soPhut / 60.0) * BANG_GIA[i].giaPerHour;
        }
    }
    return tong;
}

// ---------------------------------------------------------------------------
// tinhTienDatSan
// ---------------------------------------------------------------------------
double QuanLyDatSan::tinhTienDatSan(const DatSan& datSan) const
{
    if (datSan.getTrangThai() == "CANCELLED")
        return -1.0;

    return tinhTienTheoGio(datSan.getGioBatDau(), datSan.getGioKetThuc());
}

// ---------------------------------------------------------------------------
// apDungTienDatSan
// ---------------------------------------------------------------------------
bool QuanLyDatSan::apDungTienDatSan(int id)
{
    DatSan* ds = timTheoId(id);
    if (ds == nullptr)
        return false;

    double tien = tinhTienDatSan(*ds);
    if (tien < 0.0)
        return false; // CANCELLED or invalid times

    ds->setTongTien(tien);
    return true;
}
