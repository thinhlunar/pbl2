#pragma once

#include <string>

class TaiKhoan
{
private:
    int id;
    std::string username;
    std::string password;
    std::string role;
    int maKhachHang;
    bool trangThai;

public:
    TaiKhoan();

    TaiKhoan(
        int id,
        const std::string& username,
        const std::string& password,
        const std::string& role,
        int maKhachHang
    );

    // Getters
    int getId() const;
    const std::string& getUsername() const;
    const std::string& getPassword() const;
    const std::string& getRole() const;
    int getMaKhachHang() const;
    bool getTrangThai() const;

    // Setters (username, id, maKhachHang must NOT have setters)
    void setPassword(const std::string& password);
    void setRole(const std::string& role);
    void setTrangThai(bool trangThai);

    // Methods
    bool kiemTraMatKhau(const std::string& password) const;
};
