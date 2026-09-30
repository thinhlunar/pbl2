#include "QuanLyDichVu.h"

#include <algorithm>
#include <iostream>

// ---------------------------------------------------------------------------
// themDichVu — validated add
// ---------------------------------------------------------------------------
bool QuanLyDichVu::themDichVu(const DichVu& dichVu)
{
    // ID uniqueness
    if (timTheoId(dichVu.getId()) != nullptr)
        return false;

    // Name must not be empty
    if (dichVu.getTenDichVu().empty())
        return false;

    // Price must be non-negative
    if (dichVu.getDonGia() < 0.0)
        return false;

    danhSachDichVu.push_back(dichVu);
    return true;
}

// ---------------------------------------------------------------------------
// xoaDichVu
// ---------------------------------------------------------------------------
bool QuanLyDichVu::xoaDichVu(int id)
{
    auto it = std::find_if(
        danhSachDichVu.begin(), danhSachDichVu.end(),
        [id](const DichVu& dv) { return dv.getId() == id; });

    if (it == danhSachDichVu.end())
        return false;

    danhSachDichVu.erase(it);
    return true;
}

// ---------------------------------------------------------------------------
// suaDichVu — validated update
// ---------------------------------------------------------------------------
bool QuanLyDichVu::suaDichVu(int id,
                               const std::string& tenDichVu,
                               double donGia)
{
    DichVu* dv = timTheoId(id);
    if (dv == nullptr)
        return false;

    if (tenDichVu.empty())
        return false;

    if (donGia < 0.0)
        return false;

    dv->setTenDichVu(tenDichVu);
    dv->setDonGia(donGia);
    return true;
}

// ---------------------------------------------------------------------------
// Lookups
// ---------------------------------------------------------------------------
DichVu* QuanLyDichVu::timTheoId(int id)
{
    for (DichVu& dv : danhSachDichVu)
        if (dv.getId() == id)
            return &dv;
    return nullptr;
}

const DichVu* QuanLyDichVu::timTheoId(int id) const
{
    for (const DichVu& dv : danhSachDichVu)
        if (dv.getId() == id)
            return &dv;
    return nullptr;
}

DichVu* QuanLyDichVu::timTheoTen(const std::string& tenDichVu)
{
    for (DichVu& dv : danhSachDichVu)
        if (dv.getTenDichVu() == tenDichVu)
            return &dv;
    return nullptr;
}

const DichVu* QuanLyDichVu::timTheoTen(const std::string& tenDichVu) const
{
    for (const DichVu& dv : danhSachDichVu)
        if (dv.getTenDichVu() == tenDichVu)
            return &dv;
    return nullptr;
}

// ---------------------------------------------------------------------------
// hienThiDanhSach
// ---------------------------------------------------------------------------
void QuanLyDichVu::hienThiDanhSach() const
{
    std::cout << "=== DANH SACH DICH VU ===\n";
    for (const DichVu& dv : danhSachDichVu)
    {
        std::cout << "ID: "        << dv.getId()
                  << " | Ten: "     << dv.getTenDichVu()
                  << " | Don gia: " << dv.getDonGia()
                  << "\n";
    }
}

const std::vector<DichVu>& QuanLyDichVu::getDanhSach() const
{
    return danhSachDichVu;
}
