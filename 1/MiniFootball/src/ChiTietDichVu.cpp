#include "ChiTietDichVu.h"

ChiTietDichVu::ChiTietDichVu()
    : maDichVu(0), soLuong(0)
{
}

ChiTietDichVu::ChiTietDichVu(int maDichVu, int soLuong)
    : maDichVu(maDichVu), soLuong(soLuong)
{
}

int ChiTietDichVu::getMaDichVu() const
{
    return maDichVu;
}

int ChiTietDichVu::getSoLuong() const
{
    return soLuong;
}

void ChiTietDichVu::setSoLuong(int soLuong)
{
    this->soLuong = soLuong;
}

double ChiTietDichVu::tinhThanhTien(double donGia) const
{
    return static_cast<double>(soLuong) * donGia;
}
