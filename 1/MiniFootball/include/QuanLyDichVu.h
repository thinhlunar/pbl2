#pragma once

#include <string>
#include <vector>
#include "DichVu.h"

class QuanLyDichVu
{
private:
    std::vector<DichVu> danhSachDichVu;

public:
    QuanLyDichVu() = default;

    void themDichVu(const DichVu& dichVu);

    bool xoaDichVu(int id);

    bool suaDichVu(int id,
                   const std::string& tenDichVu,
                   double donGia);

    DichVu* timTheoId(int id);
    const DichVu* timTheoId(int id) const;

    DichVu* timTheoTen(const std::string& tenDichVu);
    const DichVu* timTheoTen(const std::string& tenDichVu) const;

    void hienThiDanhSach() const;

    const std::vector<DichVu>& getDanhSach() const;
};
