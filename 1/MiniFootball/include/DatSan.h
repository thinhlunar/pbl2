#pragma once

#include <string>

class DatSan
{
private:
    int id;
    int maSan;
    int maKhachHang;
    std::string ngay;
    std::string gioBatDau;
    std::string gioKetThuc;
    double tongTien;
    std::string trangThai;

public:
    DatSan();

    DatSan(
        int id,
        int maSan,
        int maKhachHang,
        const std::string& ngay,
        const std::string& gioBatDau,
        const std::string& gioKetThuc
    );

    // Getters
    int getId() const;
    int getMaSan() const;
    int getMaKhachHang() const;
    const std::string& getNgay() const;
    const std::string& getGioBatDau() const;
    const std::string& getGioKetThuc() const;
    double getTongTien() const;
    const std::string& getTrangThai() const;

    // Setters (id, maSan, maKhachHang must NOT have setters)
    void setNgay(const std::string& ngay);
    void setGioBatDau(const std::string& gioBatDau);
    void setGioKetThuc(const std::string& gioKetThuc);
    void setTongTien(double tongTien);
    void setTrangThai(const std::string& trangThai);
};
