#pragma once

#include <string>

class DichVu
{
private:
    int id;
    std::string tenDichVu;
    double donGia;

public:
    DichVu();

    DichVu(
        int id,
        const std::string& tenDichVu,
        double donGia
    );

    // Getters
    int getId() const;
    const std::string& getTenDichVu() const;
    double getDonGia() const;

    // Setters (id must NOT have a setter)
    void setTenDichVu(const std::string& tenDichVu);
    void setDonGia(double donGia);
};
