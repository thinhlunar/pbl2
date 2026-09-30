#pragma once

#include <string>

class SanBong
{
private:
    int id;
    std::string tenSan;
    std::string loaiSan;
    double giaMoiGio;
    bool trangThai;

public:
    SanBong();

    SanBong(
        int id,
        const std::string& tenSan,
        const std::string& loaiSan,
        double giaMoiGio
    );

    // Getters
    int getId() const;
    const std::string& getTenSan() const;
    const std::string& getLoaiSan() const;
    double getGiaMoiGio() const;
    bool getTrangThai() const;

    // Setters (id must NOT have a setter)
    void setTenSan(const std::string& tenSan);
    void setLoaiSan(const std::string& loaiSan);
    void setGiaMoiGio(double giaMoiGio);
    void setTrangThai(bool trangThai);
};
