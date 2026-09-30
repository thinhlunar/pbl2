#pragma once

#include <string>
#include <vector>
#include "SanBong.h"

class QuanLySan
{
private:
    std::vector<SanBong> danhSachSan;

public:
    QuanLySan() = default;

    void themSan(const SanBong& sanBong);

    bool xoaSan(int id);

    // Cập nhật tên, loại và giá; trả về true nếu thành công
    bool suaSan(int id,
                const std::string& tenSan,
                const std::string& loaiSan,
                double giaMoiGio);

    SanBong* timTheoId(int id);
    const SanBong* timTheoId(int id) const;

    // Tìm theo tên sân (so sánh chính xác)
    SanBong* timTheoTen(const std::string& tenSan);
    const SanBong* timTheoTen(const std::string& tenSan) const;

    bool tonTai(int id) const;

    // Cập nhật trạng thái hoạt động của sân; trả về true nếu thành công
    bool capNhatTrangThai(int id, bool trangThai);

    void hienThiDanhSach() const;

    const std::vector<SanBong>& getDanhSach() const;
};
