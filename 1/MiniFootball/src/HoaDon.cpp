#include "HoaDon.h"

#include <algorithm>

HoaDon::HoaDon()
    : id(0), maDatSan(0), tienSan(0.0), tienDichVu(0.0),
      tongTien(0.0), ngayThanhToan(""), trangThai("UNPAID")
{
}

HoaDon::HoaDon(int id, int maDatSan, double tienSan)
    : id(id), maDatSan(maDatSan), tienSan(tienSan), tienDichVu(0.0),
      tongTien(tienSan), ngayThanhToan(""), trangThai("UNPAID")
{
}

int HoaDon::getId() const
{
    return id;
}

int HoaDon::getMaDatSan() const
{
    return maDatSan;
}

double HoaDon::getTienSan() const
{
    return tienSan;
}

double HoaDon::getTienDichVu() const
{
    return tienDichVu;
}

double HoaDon::getTongTien() const
{
    return tongTien;
}

const std::string& HoaDon::getNgayThanhToan() const
{
    return ngayThanhToan;
}

const std::string& HoaDon::getTrangThai() const
{
    return trangThai;
}

const std::vector<ChiTietDichVu>& HoaDon::getChiTietDichVu() const
{
    return chiTietDichVu;
}

void HoaDon::setTienSan(double tienSan)
{
    this->tienSan = tienSan;
}

void HoaDon::setTienDichVu(double tienDichVu)
{
    this->tienDichVu = tienDichVu;
}

void HoaDon::setNgayThanhToan(const std::string& ngayThanhToan)
{
    this->ngayThanhToan = ngayThanhToan;
}

void HoaDon::setTrangThai(const std::string& trangThai)
{
    this->trangThai = trangThai;
}

void HoaDon::themDichVu(const ChiTietDichVu& chiTiet)
{
    chiTietDichVu.push_back(chiTiet);
}

void HoaDon::xoaDichVu(int maDichVu)
{
    chiTietDichVu.erase(
        std::remove_if(
            chiTietDichVu.begin(),
            chiTietDichVu.end(),
            [maDichVu](const ChiTietDichVu& ct)
            {
                return ct.getMaDichVu() == maDichVu;
            }),
        chiTietDichVu.end());
}

double HoaDon::tinhTienDichVu(const std::vector<DichVu>& danhSachDichVu) const
{
    double total = 0.0;
    for (const ChiTietDichVu& ct : chiTietDichVu)
    {
        for (const DichVu& dv : danhSachDichVu)
        {
            if (dv.getId() == ct.getMaDichVu())
            {
                total += ct.tinhThanhTien(dv.getDonGia());
                break;
            }
        }
    }
    return total;
}

void HoaDon::tinhTongTien()
{
    tongTien = tienSan + tienDichVu;
}
