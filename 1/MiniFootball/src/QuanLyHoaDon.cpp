#include "QuanLyHoaDon.h"
#include "QuanLyDichVu.h"
#include "QuanLyDatSan.h"

#include <algorithm>
#include <iostream>
#include <string>

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
QuanLyHoaDon::QuanLyHoaDon(QuanLyDichVu& quanLyDichVu,
                             QuanLyDatSan& quanLyDatSan)
    : quanLyDichVu(quanLyDichVu), quanLyDatSan(quanLyDatSan)
{
}

// ---------------------------------------------------------------------------
// Private: recalculate tienDichVu and tongTien
// ---------------------------------------------------------------------------
void QuanLyHoaDon::capNhatTong(HoaDon& hd)
{
    double tienDV = hd.tinhTienDichVu(quanLyDichVu.getDanhSach());
    hd.setTienDichVu(tienDV);
    hd.tinhTongTien(); // tongTien = tienSan + tienDichVu
}

// ---------------------------------------------------------------------------
// taoHoaDon — validated creation from a booking
// ---------------------------------------------------------------------------
bool QuanLyHoaDon::taoHoaDon(int maHoaDon, int maDatSan)
{
    // Invoice ID must be unique
    if (timTheoId(maHoaDon) != nullptr)
        return false;

    // Booking must exist
    const DatSan* ds = quanLyDatSan.timTheoId(maDatSan);
    if (ds == nullptr)
        return false;

    // Cancelled bookings cannot have an invoice
    if (ds->getTrangThai() == "CANCELLED")
        return false;

    // No duplicate invoice for the same booking
    if (timTheoMaDatSan(maDatSan) != nullptr)
        return false;

    // tienSan comes from the booking's already-calculated tongTien (Phase 5)
    HoaDon hd(maHoaDon, maDatSan, ds->getTongTien());
    danhSachHoaDon.push_back(hd);
    return true;
}

// ---------------------------------------------------------------------------
// themHoaDon — raw add (no booking validation, for file loading)
// ---------------------------------------------------------------------------
void QuanLyHoaDon::themHoaDon(const HoaDon& hoaDon)
{
    danhSachHoaDon.push_back(hoaDon);
}

// ---------------------------------------------------------------------------
// xoaHoaDon
// ---------------------------------------------------------------------------
bool QuanLyHoaDon::xoaHoaDon(int id)
{
    auto it = std::find_if(
        danhSachHoaDon.begin(), danhSachHoaDon.end(),
        [id](const HoaDon& hd) { return hd.getId() == id; });

    if (it == danhSachHoaDon.end())
        return false;

    danhSachHoaDon.erase(it);
    return true;
}

// ---------------------------------------------------------------------------
// themDichVuVaoHoaDon — add or merge service line
// ---------------------------------------------------------------------------
bool QuanLyHoaDon::themDichVuVaoHoaDon(int maHoaDon, int maDichVu, int soLuong)
{
    // Quantity must be positive
    if (soLuong <= 0)
        return false;

    // Invoice must exist
    HoaDon* hd = timTheoId(maHoaDon);
    if (hd == nullptr)
        return false;

    // Service must exist
    if (quanLyDichVu.timTheoId(maDichVu) == nullptr)
        return false;

    // Check if service already exists in this invoice
    const std::vector<ChiTietDichVu>& lines = hd->getChiTietDichVu();
    bool found = false;
    for (const ChiTietDichVu& ct : lines)
    {
        if (ct.getMaDichVu() == maDichVu)
        {
            found = true;
            break;
        }
    }

    if (found)
    {
        // Increase quantity on the existing line.
        // HoaDon does not expose a non-const reference to its lines,
        // so we remove and re-add with updated quantity.
        int currentQty = 0;
        for (const ChiTietDichVu& ct : lines)
            if (ct.getMaDichVu() == maDichVu)
            { currentQty = ct.getSoLuong(); break; }

        hd->xoaDichVu(maDichVu);
        hd->themDichVu(ChiTietDichVu(maDichVu, currentQty + soLuong));
    }
    else
    {
        hd->themDichVu(ChiTietDichVu(maDichVu, soLuong));
    }

    capNhatTong(*hd);
    return true;
}

// ---------------------------------------------------------------------------
// xoaDichVuKhoiHoaDon
// ---------------------------------------------------------------------------
bool QuanLyHoaDon::xoaDichVuKhoiHoaDon(int maHoaDon, int maDichVu)
{
    HoaDon* hd = timTheoId(maHoaDon);
    if (hd == nullptr)
        return false;

    // Check the line exists before removing
    bool found = false;
    for (const ChiTietDichVu& ct : hd->getChiTietDichVu())
        if (ct.getMaDichVu() == maDichVu) { found = true; break; }

    if (!found)
        return false;

    hd->xoaDichVu(maDichVu);
    capNhatTong(*hd);
    return true;
}

// ---------------------------------------------------------------------------
// capNhatSoLuongDichVu
// ---------------------------------------------------------------------------
bool QuanLyHoaDon::capNhatSoLuongDichVu(int maHoaDon, int maDichVu, int soLuong)
{
    if (soLuong <= 0)
        return false;

    HoaDon* hd = timTheoId(maHoaDon);
    if (hd == nullptr)
        return false;

    // Find the line
    bool found = false;
    for (const ChiTietDichVu& ct : hd->getChiTietDichVu())
        if (ct.getMaDichVu() == maDichVu) { found = true; break; }

    if (!found)
        return false;

    // Replace: remove old line, add new one with updated quantity
    hd->xoaDichVu(maDichVu);
    hd->themDichVu(ChiTietDichVu(maDichVu, soLuong));
    capNhatTong(*hd);
    return true;
}

// ---------------------------------------------------------------------------
// thanhToan — payment with UNPAID guard
// ---------------------------------------------------------------------------
bool QuanLyHoaDon::thanhToan(int maHoaDon, const std::string& ngayThanhToan)
{
    if (ngayThanhToan.empty())
        return false;

    HoaDon* hd = timTheoId(maHoaDon);
    if (hd == nullptr)
        return false;

    // Only UNPAID invoices can be paid
    if (hd->getTrangThai() != "UNPAID")
        return false;

    hd->setTrangThai("PAID");
    hd->setNgayThanhToan(ngayThanhToan);
    return true;
}

// ---------------------------------------------------------------------------
// Lookups
// ---------------------------------------------------------------------------
HoaDon* QuanLyHoaDon::timTheoId(int id)
{
    for (HoaDon& hd : danhSachHoaDon)
        if (hd.getId() == id) return &hd;
    return nullptr;
}

const HoaDon* QuanLyHoaDon::timTheoId(int id) const
{
    for (const HoaDon& hd : danhSachHoaDon)
        if (hd.getId() == id) return &hd;
    return nullptr;
}

HoaDon* QuanLyHoaDon::timTheoMaDatSan(int maDatSan)
{
    for (HoaDon& hd : danhSachHoaDon)
        if (hd.getMaDatSan() == maDatSan) return &hd;
    return nullptr;
}

const HoaDon* QuanLyHoaDon::timTheoMaDatSan(int maDatSan) const
{
    for (const HoaDon& hd : danhSachHoaDon)
        if (hd.getMaDatSan() == maDatSan) return &hd;
    return nullptr;
}

std::vector<HoaDon*> QuanLyHoaDon::layHoaDonChuaThanhToan()
{
    std::vector<HoaDon*> result;
    for (HoaDon& hd : danhSachHoaDon)
        if (hd.getTrangThai() == "UNPAID") result.push_back(&hd);
    return result;
}

std::vector<const HoaDon*> QuanLyHoaDon::layHoaDonChuaThanhToan() const
{
    std::vector<const HoaDon*> result;
    for (const HoaDon& hd : danhSachHoaDon)
        if (hd.getTrangThai() == "UNPAID") result.push_back(&hd);
    return result;
}

std::vector<HoaDon*> QuanLyHoaDon::layHoaDonDaThanhToan()
{
    std::vector<HoaDon*> result;
    for (HoaDon& hd : danhSachHoaDon)
        if (hd.getTrangThai() == "PAID") result.push_back(&hd);
    return result;
}

std::vector<const HoaDon*> QuanLyHoaDon::layHoaDonDaThanhToan() const
{
    std::vector<const HoaDon*> result;
    for (const HoaDon& hd : danhSachHoaDon)
        if (hd.getTrangThai() == "PAID") result.push_back(&hd);
    return result;
}

// ---------------------------------------------------------------------------
// hienThiDanhSach
// ---------------------------------------------------------------------------
void QuanLyHoaDon::hienThiDanhSach() const
{
    std::cout << "=== DANH SACH HOA DON ===\n";
    for (const HoaDon& hd : danhSachHoaDon)
    {
        std::cout << "ID: "             << hd.getId()
                  << " | Ma dat san: "   << hd.getMaDatSan()
                  << " | Tien san: "     << hd.getTienSan()
                  << " | Tien DV: "      << hd.getTienDichVu()
                  << " | Tong: "         << hd.getTongTien()
                  << " | Trang thai: "   << hd.getTrangThai()
                  << " | Ngay TT: "      << hd.getNgayThanhToan()
                  << "\n";
    }
}

// ---------------------------------------------------------------------------
// Revenue helpers
// ---------------------------------------------------------------------------
static bool trichThangNam(const std::string& ngay, int& thang, int& nam)
{
    if (ngay.size() != 10) return false;
    try
    {
        thang = std::stoi(ngay.substr(3, 2));
        nam   = std::stoi(ngay.substr(6, 4));
    }
    catch (...) { return false; }
    return true;
}

double QuanLyHoaDon::tinhDoanhThuTheoNgay(const std::string& ngay) const
{
    double tong = 0.0;
    for (const HoaDon& hd : danhSachHoaDon)
        if (hd.getTrangThai() == "PAID" && hd.getNgayThanhToan() == ngay)
            tong += hd.getTongTien();
    return tong;
}

double QuanLyHoaDon::tinhDoanhThuTheoThang(int thang, int nam) const
{
    double tong = 0.0;
    for (const HoaDon& hd : danhSachHoaDon)
    {
        if (hd.getTrangThai() != "PAID") continue;
        int t = 0, n = 0;
        if (!trichThangNam(hd.getNgayThanhToan(), t, n)) continue;
        if (t == thang && n == nam) tong += hd.getTongTien();
    }
    return tong;
}

double QuanLyHoaDon::tinhTongDoanhThu() const
{
    double tong = 0.0;
    for (const HoaDon& hd : danhSachHoaDon)
        if (hd.getTrangThai() == "PAID") tong += hd.getTongTien();
    return tong;
}

const std::vector<HoaDon>& QuanLyHoaDon::getDanhSach() const
{
    return danhSachHoaDon;
}
