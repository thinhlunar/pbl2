#include "DatSan.h"

DatSan::DatSan()
    : id(0), maSan(0), maKhachHang(0), ngay(""), gioBatDau(""),
      gioKetThuc(""), tongTien(0.0), trangThai("BOOKED")
{
}

DatSan::DatSan(
    int id,
    int maSan,
    int maKhachHang,
    const std::string& ngay,
    const std::string& gioBatDau,
    const std::string& gioKetThuc)
    : id(id), maSan(maSan), maKhachHang(maKhachHang), ngay(ngay),
      gioBatDau(gioBatDau), gioKetThuc(gioKetThuc), tongTien(0.0), trangThai("BOOKED")
{
}

int DatSan::getId() const
{
    return id;
}

int DatSan::getMaSan() const
{
    return maSan;
}

int DatSan::getMaKhachHang() const
{
    return maKhachHang;
}

const std::string& DatSan::getNgay() const
{
    return ngay;
}

const std::string& DatSan::getGioBatDau() const
{
    return gioBatDau;
}

const std::string& DatSan::getGioKetThuc() const
{
    return gioKetThuc;
}

double DatSan::getTongTien() const
{
    return tongTien;
}

const std::string& DatSan::getTrangThai() const
{
    return trangThai;
}

void DatSan::setNgay(const std::string& ngay)
{
    this->ngay = ngay;
}

void DatSan::setGioBatDau(const std::string& gioBatDau)
{
    this->gioBatDau = gioBatDau;
}

void DatSan::setGioKetThuc(const std::string& gioKetThuc)
{
    this->gioKetThuc = gioKetThuc;
}

void DatSan::setTongTien(double tongTien)
{
    this->tongTien = tongTien;
}

void DatSan::setTrangThai(const std::string& trangThai)
{
    this->trangThai = trangThai;
}
