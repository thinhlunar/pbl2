#include "DichVu.h"

DichVu::DichVu()
    : id(0), tenDichVu(""), donGia(0.0)
{
}

DichVu::DichVu(
    int id,
    const std::string& tenDichVu,
    double donGia)
    : id(id), tenDichVu(tenDichVu), donGia(donGia)
{
}

int DichVu::getId() const
{
    return id;
}

const std::string& DichVu::getTenDichVu() const
{
    return tenDichVu;
}

double DichVu::getDonGia() const
{
    return donGia;
}

void DichVu::setTenDichVu(const std::string& tenDichVu)
{
    this->tenDichVu = tenDichVu;
}

void DichVu::setDonGia(double donGia)
{
    this->donGia = donGia;
}
