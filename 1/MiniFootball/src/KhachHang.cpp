#include "KhachHang.h"

KhachHang::KhachHang()
    : id(0), hoTen(""), soDienThoai(""), email("")
{
}

KhachHang::KhachHang(
    int id,
    const std::string& hoTen,
    const std::string& soDienThoai,
    const std::string& email)
    : id(id), hoTen(hoTen), soDienThoai(soDienThoai), email(email)
{
}

int KhachHang::getId() const
{
    return id;
}

const std::string& KhachHang::getHoTen() const
{
    return hoTen;
}

const std::string& KhachHang::getSoDienThoai() const
{
    return soDienThoai;
}

const std::string& KhachHang::getEmail() const
{
    return email;
}

void KhachHang::setHoTen(const std::string& hoTen)
{
    this->hoTen = hoTen;
}

void KhachHang::setSoDienThoai(const std::string& soDienThoai)
{
    this->soDienThoai = soDienThoai;
}

void KhachHang::setEmail(const std::string& email)
{
    this->email = email;
}
