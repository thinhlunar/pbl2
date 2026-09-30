#include "SanBong.h"

SanBong::SanBong()
    : id(0), tenSan(""), loaiSan(""), giaMoiGio(0.0), trangThai(true)
{
}

SanBong::SanBong(
    int id,
    const std::string& tenSan,
    const std::string& loaiSan,
    double giaMoiGio)
    : id(id), tenSan(tenSan), loaiSan(loaiSan), giaMoiGio(giaMoiGio), trangThai(true)
{
}

int SanBong::getId() const
{
    return id;
}

const std::string& SanBong::getTenSan() const
{
    return tenSan;
}

const std::string& SanBong::getLoaiSan() const
{
    return loaiSan;
}

double SanBong::getGiaMoiGio() const
{
    return giaMoiGio;
}

bool SanBong::getTrangThai() const
{
    return trangThai;
}

void SanBong::setTenSan(const std::string& tenSan)
{
    this->tenSan = tenSan;
}

void SanBong::setLoaiSan(const std::string& loaiSan)
{
    this->loaiSan = loaiSan;
}

void SanBong::setGiaMoiGio(double giaMoiGio)
{
    this->giaMoiGio = giaMoiGio;
}

void SanBong::setTrangThai(bool trangThai)
{
    this->trangThai = trangThai;
}
