#pragma once

#include <string>
#include <vector>
#include "ChiTietDichVu.h"
#include "DichVu.h"

class HoaDon
{
private:
    int id;
    int maDatSan;
    double tienSan;
    double tienDichVu;
    double tongTien;
    std::string ngayThanhToan;
    std::string trangThai;
    std::vector<ChiTietDichVu> chiTietDichVu;

public:
    HoaDon();

    HoaDon(
        int id,
        int maDatSan,
        double tienSan
    );

    // Getters
    int getId() const;
    int getMaDatSan() const;
    double getTienSan() const;
    double getTienDichVu() const;
    double getTongTien() const;
    const std::string& getNgayThanhToan() const;
    const std::string& getTrangThai() const;
    const std::vector<ChiTietDichVu>& getChiTietDichVu() const;

    // Setters (id, maDatSan must NOT have setters)
    void setTienSan(double tienSan);
    void setTienDichVu(double tienDichVu);
    void setNgayThanhToan(const std::string& ngayThanhToan);
    void setTrangThai(const std::string& trangThai);

    // Methods
    void themDichVu(const ChiTietDichVu& chiTiet);
    void xoaDichVu(int maDichVu);
    double tinhTienDichVu(const std::vector<DichVu>& danhSachDichVu) const;
    void tinhTongTien();
};
