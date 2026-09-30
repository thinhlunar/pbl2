#include "QuanLyKhachHang.h"

#include <algorithm>
#include <iostream>

void QuanLyKhachHang::themKhachHang(const KhachHang& khachHang)
{
    danhSachKhachHang.push_back(khachHang);
}

bool QuanLyKhachHang::xoaKhachHang(int id)
{
    auto it = std::find_if(
        danhSachKhachHang.begin(), danhSachKhachHang.end(),
        [id](const KhachHang& kh) { return kh.getId() == id; });

    if (it == danhSachKhachHang.end())
        return false;

    danhSachKhachHang.erase(it);
    return true;
}

bool QuanLyKhachHang::suaKhachHang(int id,
                                    const std::string& hoTen,
                                    const std::string& soDienThoai,
                                    const std::string& email)
{
    KhachHang* kh = timTheoId(id);
    if (kh == nullptr)
        return false;

    kh->setHoTen(hoTen);
    kh->setSoDienThoai(soDienThoai);
    kh->setEmail(email);
    return true;
}

KhachHang* QuanLyKhachHang::timTheoId(int id)
{
    for (KhachHang& kh : danhSachKhachHang)
        if (kh.getId() == id)
            return &kh;
    return nullptr;
}

const KhachHang* QuanLyKhachHang::timTheoId(int id) const
{
    for (const KhachHang& kh : danhSachKhachHang)
        if (kh.getId() == id)
            return &kh;
    return nullptr;
}

KhachHang* QuanLyKhachHang::timTheoHoTen(const std::string& hoTen)
{
    for (KhachHang& kh : danhSachKhachHang)
        if (kh.getHoTen() == hoTen)
            return &kh;
    return nullptr;
}

const KhachHang* QuanLyKhachHang::timTheoHoTen(const std::string& hoTen) const
{
    for (const KhachHang& kh : danhSachKhachHang)
        if (kh.getHoTen() == hoTen)
            return &kh;
    return nullptr;
}

KhachHang* QuanLyKhachHang::timTheoSoDienThoai(const std::string& soDienThoai)
{
    for (KhachHang& kh : danhSachKhachHang)
        if (kh.getSoDienThoai() == soDienThoai)
            return &kh;
    return nullptr;
}

const KhachHang* QuanLyKhachHang::timTheoSoDienThoai(const std::string& soDienThoai) const
{
    for (const KhachHang& kh : danhSachKhachHang)
        if (kh.getSoDienThoai() == soDienThoai)
            return &kh;
    return nullptr;
}

bool QuanLyKhachHang::tonTai(int id) const
{
    return timTheoId(id) != nullptr;
}

void QuanLyKhachHang::hienThiDanhSach() const
{
    std::cout << "=== DANH SACH KHACH HANG ===\n";
    for (const KhachHang& kh : danhSachKhachHang)
    {
        std::cout << "ID: "   << kh.getId()
                  << " | Ho ten: " << kh.getHoTen()
                  << " | SDT: "    << kh.getSoDienThoai()
                  << " | Email: "  << kh.getEmail()
                  << "\n";
    }
}

const std::vector<KhachHang>& QuanLyKhachHang::getDanhSach() const
{
    return danhSachKhachHang;
}
