#include "QuanLyTaiKhoan.h"

#include <algorithm>
#include <iostream>

void QuanLyTaiKhoan::themTaiKhoan(const TaiKhoan& taiKhoan)
{
    danhSachTaiKhoan.push_back(taiKhoan);
}

bool QuanLyTaiKhoan::xoaTaiKhoan(int id)
{
    auto it = std::find_if(
        danhSachTaiKhoan.begin(), danhSachTaiKhoan.end(),
        [id](const TaiKhoan& tk) { return tk.getId() == id; });

    if (it == danhSachTaiKhoan.end())
        return false;

    danhSachTaiKhoan.erase(it);
    return true;
}

TaiKhoan* QuanLyTaiKhoan::timTheoId(int id)
{
    for (TaiKhoan& tk : danhSachTaiKhoan)
        if (tk.getId() == id)
            return &tk;
    return nullptr;
}

const TaiKhoan* QuanLyTaiKhoan::timTheoId(int id) const
{
    for (const TaiKhoan& tk : danhSachTaiKhoan)
        if (tk.getId() == id)
            return &tk;
    return nullptr;
}

TaiKhoan* QuanLyTaiKhoan::timTheoUsername(const std::string& username)
{
    for (TaiKhoan& tk : danhSachTaiKhoan)
        if (tk.getUsername() == username)
            return &tk;
    return nullptr;
}

const TaiKhoan* QuanLyTaiKhoan::timTheoUsername(const std::string& username) const
{
    for (const TaiKhoan& tk : danhSachTaiKhoan)
        if (tk.getUsername() == username)
            return &tk;
    return nullptr;
}

bool QuanLyTaiKhoan::usernameTonTai(const std::string& username) const
{
    return timTheoUsername(username) != nullptr;
}

TaiKhoan* QuanLyTaiKhoan::dangNhap(const std::string& username,
                                    const std::string& password)
{
    TaiKhoan* tk = timTheoUsername(username);
    if (tk == nullptr)
        return nullptr;
    if (!tk->getTrangThai())
        return nullptr; // tài khoản bị khóa
    if (!tk->kiemTraMatKhau(password))
        return nullptr;
    return tk;
}

bool QuanLyTaiKhoan::doiMatKhau(int id, const std::string& matKhauMoi)
{
    TaiKhoan* tk = timTheoId(id);
    if (tk == nullptr)
        return false;
    tk->setPassword(matKhauMoi);
    return true;
}

bool QuanLyTaiKhoan::khoaTaiKhoan(int id)
{
    TaiKhoan* tk = timTheoId(id);
    if (tk == nullptr)
        return false;
    tk->setTrangThai(false);
    return true;
}

bool QuanLyTaiKhoan::moKhoaTaiKhoan(int id)
{
    TaiKhoan* tk = timTheoId(id);
    if (tk == nullptr)
        return false;
    tk->setTrangThai(true);
    return true;
}

void QuanLyTaiKhoan::hienThiDanhSach() const
{
    std::cout << "=== DANH SACH TAI KHOAN ===\n";
    for (const TaiKhoan& tk : danhSachTaiKhoan)
    {
        std::cout << "ID: "         << tk.getId()
                  << " | Username: " << tk.getUsername()
                  << " | Role: "     << tk.getRole()
                  << " | Trang thai: " << (tk.getTrangThai() ? "Hoat dong" : "Bi khoa")
                  << "\n";
    }
}

const std::vector<TaiKhoan>& QuanLyTaiKhoan::getDanhSach() const
{
    return danhSachTaiKhoan;
}
