#include "QuanLyHoaDon.h"

#include <algorithm>
#include <iostream>
#include <string>

void QuanLyHoaDon::themHoaDon(const HoaDon& hoaDon)
{
    danhSachHoaDon.push_back(hoaDon);
}

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

HoaDon* QuanLyHoaDon::timTheoId(int id)
{
    for (HoaDon& hd : danhSachHoaDon)
        if (hd.getId() == id)
            return &hd;
    return nullptr;
}

const HoaDon* QuanLyHoaDon::timTheoId(int id) const
{
    for (const HoaDon& hd : danhSachHoaDon)
        if (hd.getId() == id)
            return &hd;
    return nullptr;
}

HoaDon* QuanLyHoaDon::timTheoMaDatSan(int maDatSan)
{
    for (HoaDon& hd : danhSachHoaDon)
        if (hd.getMaDatSan() == maDatSan)
            return &hd;
    return nullptr;
}

const HoaDon* QuanLyHoaDon::timTheoMaDatSan(int maDatSan) const
{
    for (const HoaDon& hd : danhSachHoaDon)
        if (hd.getMaDatSan() == maDatSan)
            return &hd;
    return nullptr;
}

bool QuanLyHoaDon::thanhToan(int id, const std::string& ngayThanhToan)
{
    HoaDon* hd = timTheoId(id);
    if (hd == nullptr)
        return false;
    hd->setTrangThai("PAID");
    hd->setNgayThanhToan(ngayThanhToan);
    return true;
}

void QuanLyHoaDon::hienThiDanhSach() const
{
    std::cout << "=== DANH SACH HOA DON ===\n";
    for (const HoaDon& hd : danhSachHoaDon)
    {
        std::cout << "ID: "            << hd.getId()
                  << " | Ma dat san: "  << hd.getMaDatSan()
                  << " | Tien san: "    << hd.getTienSan()
                  << " | Tien dich vu: "<< hd.getTienDichVu()
                  << " | Tong: "        << hd.getTongTien()
                  << " | Trang thai: "  << hd.getTrangThai()
                  << " | Ngay TT: "     << hd.getNgayThanhToan()
                  << "\n";
    }
}

// Helper: trích tháng và năm từ chuỗi "DD/MM/YYYY"
static bool trichThangNam(const std::string& ngay, int& thang, int& nam)
{
    // Định dạng mong đợi: DD/MM/YYYY (length == 10)
    if (ngay.size() != 10)
        return false;
    try
    {
        thang = std::stoi(ngay.substr(3, 2));
        nam   = std::stoi(ngay.substr(6, 4));
    }
    catch (...)
    {
        return false;
    }
    return true;
}

double QuanLyHoaDon::tinhDoanhThuTheoNgay(const std::string& ngay) const
{
    double tong = 0.0;
    for (const HoaDon& hd : danhSachHoaDon)
    {
        if (hd.getTrangThai() == "PAID" && hd.getNgayThanhToan() == ngay)
            tong += hd.getTongTien();
    }
    return tong;
}

double QuanLyHoaDon::tinhDoanhThuTheoThang(int thang, int nam) const
{
    double tong = 0.0;
    for (const HoaDon& hd : danhSachHoaDon)
    {
        if (hd.getTrangThai() != "PAID")
            continue;

        int t = 0, n = 0;
        if (!trichThangNam(hd.getNgayThanhToan(), t, n))
            continue;

        if (t == thang && n == nam)
            tong += hd.getTongTien();
    }
    return tong;
}

double QuanLyHoaDon::tinhTongDoanhThu() const
{
    double tong = 0.0;
    for (const HoaDon& hd : danhSachHoaDon)
        if (hd.getTrangThai() == "PAID")
            tong += hd.getTongTien();
    return tong;
}

const std::vector<HoaDon>& QuanLyHoaDon::getDanhSach() const
{
    return danhSachHoaDon;
}
