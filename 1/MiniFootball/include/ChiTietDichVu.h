#pragma once

class ChiTietDichVu
{
private:
    int maDichVu;
    int soLuong;

public:
    ChiTietDichVu();

    ChiTietDichVu(int maDichVu, int soLuong);

    // Getters
    int getMaDichVu() const;
    int getSoLuong() const;

    // Setter (maDichVu must NOT have a setter)
    void setSoLuong(int soLuong);

    // Methods
    double tinhThanhTien(double donGia) const;
};
