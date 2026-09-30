#pragma once

#include <string>

class KhachHang
{
private:
    int id;
    std::string hoTen;
    std::string soDienThoai;
    std::string email;

public:
    KhachHang();

    KhachHang(
        int id,
        const std::string& hoTen,
        const std::string& soDienThoai,
        const std::string& email
    );

    // Getters
    int getId() const;
    const std::string& getHoTen() const;
    const std::string& getSoDienThoai() const;
    const std::string& getEmail() const;

    // Setters (id must NOT have a setter)
    void setHoTen(const std::string& hoTen);
    void setSoDienThoai(const std::string& soDienThoai);
    void setEmail(const std::string& email);
};
