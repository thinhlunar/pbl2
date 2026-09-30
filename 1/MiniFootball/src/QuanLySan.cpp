#include "QuanLySan.h"

#include <algorithm>
#include <iostream>

void QuanLySan::themSan(const SanBong& sanBong)
{
    danhSachSan.push_back(sanBong);
}

bool QuanLySan::xoaSan(int id)
{
    auto it = std::find_if(
        danhSachSan.begin(), danhSachSan.end(),
        [id](const SanBong& sb) { return sb.getId() == id; });

    if (it == danhSachSan.end())
        return false;

    danhSachSan.erase(it);
    return true;
}

bool QuanLySan::suaSan(int id,
                       const std::string& tenSan,
                       const std::string& loaiSan,
                       double giaMoiGio)
{
    SanBong* sb = timTheoId(id);
    if (sb == nullptr)
        return false;

    sb->setTenSan(tenSan);
    sb->setLoaiSan(loaiSan);
    sb->setGiaMoiGio(giaMoiGio);
    return true;
}

SanBong* QuanLySan::timTheoId(int id)
{
    for (SanBong& sb : danhSachSan)
        if (sb.getId() == id)
            return &sb;
    return nullptr;
}

const SanBong* QuanLySan::timTheoId(int id) const
{
    for (const SanBong& sb : danhSachSan)
        if (sb.getId() == id)
            return &sb;
    return nullptr;
}

SanBong* QuanLySan::timTheoTen(const std::string& tenSan)
{
    for (SanBong& sb : danhSachSan)
        if (sb.getTenSan() == tenSan)
            return &sb;
    return nullptr;
}

const SanBong* QuanLySan::timTheoTen(const std::string& tenSan) const
{
    for (const SanBong& sb : danhSachSan)
        if (sb.getTenSan() == tenSan)
            return &sb;
    return nullptr;
}

bool QuanLySan::tonTai(int id) const
{
    return timTheoId(id) != nullptr;
}

bool QuanLySan::capNhatTrangThai(int id, bool trangThai)
{
    SanBong* sb = timTheoId(id);
    if (sb == nullptr)
        return false;

    sb->setTrangThai(trangThai);
    return true;
}

void QuanLySan::hienThiDanhSach() const
{
    std::cout << "=== DANH SACH SAN BONG ===\n";
    for (const SanBong& sb : danhSachSan)
    {
        std::cout << "ID: "         << sb.getId()
                  << " | Ten san: "  << sb.getTenSan()
                  << " | Loai: "     << sb.getLoaiSan()
                  << " | Gia/gio: "  << sb.getGiaMoiGio()
                  << " | Trang thai: " << (sb.getTrangThai() ? "Hoat dong" : "Ngung hoat dong")
                  << "\n";
    }
}

const std::vector<SanBong>& QuanLySan::getDanhSach() const
{
    return danhSachSan;
}
