#include "QuanLyDichVu.h"

#include <algorithm>
#include <iostream>

void QuanLyDichVu::themDichVu(const DichVu& dichVu)
{
    danhSachDichVu.push_back(dichVu);
}

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

bool QuanLyDichVu::suaDichVu(int id,
                               const std::string& tenDichVu,
                               double donGia)
{
    DichVu* dv = timTheoId(id);
    if (dv == nullptr)
        return false;

    dv->setTenDichVu(tenDichVu);
    dv->setDonGia(donGia);
    return true;
}

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

void QuanLyDichVu::hienThiDanhSach() const
{
    std::cout << "=== DANH SACH DICH VU ===\n";
    for (const DichVu& dv : danhSachDichVu)
    {
        std::cout << "ID: "         << dv.getId()
                  << " | Ten: "      << dv.getTenDichVu()
                  << " | Don gia: "  << dv.getDonGia()
                  << "\n";
    }
}

const std::vector<DichVu>& QuanLyDichVu::getDanhSach() const
{
    return danhSachDichVu;
}
