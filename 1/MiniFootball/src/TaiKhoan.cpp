#include "TaiKhoan.h"

TaiKhoan::TaiKhoan()
    : id(0), username(""), password(""), role(""), maKhachHang(-1), trangThai(true)
{
}

TaiKhoan::TaiKhoan(
    int id,
    const std::string& username,
    const std::string& password,
    const std::string& role,
    int maKhachHang)
    : id(id), username(username), password(password), role(role),
      maKhachHang(maKhachHang), trangThai(true)
{
}

int TaiKhoan::getId() const
{
    return id;
}

const std::string& TaiKhoan::getUsername() const
{
    return username;
}

const std::string& TaiKhoan::getPassword() const
{
    return password;
}

const std::string& TaiKhoan::getRole() const
{
    return role;
}

int TaiKhoan::getMaKhachHang() const
{
    return maKhachHang;
}

bool TaiKhoan::getTrangThai() const
{
    return trangThai;
}

void TaiKhoan::setPassword(const std::string& password)
{
    this->password = password;
}

void TaiKhoan::setRole(const std::string& role)
{
    this->role = role;
}

void TaiKhoan::setTrangThai(bool trangThai)
{
    this->trangThai = trangThai;
}

bool TaiKhoan::kiemTraMatKhau(const std::string& password) const
{
    return this->password == password;
}
