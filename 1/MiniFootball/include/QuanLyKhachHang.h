#pragma once

#include <string>
#include <vector>
#include "KhachHang.h"

class QuanLyKhachHang
{
private:
    std::vector<KhachHang> danhSachKhachHang;

public:
    QuanLyKhachHang() = default;

    void themKhachHang(const KhachHang& khachHang);

    // Trả về true nếu tìm và xóa thành công
    bool xoaKhachHang(int id);

    // Cập nhật thông tin khách hàng theo id; trả về true nếu thành công
    bool suaKhachHang(int id,
                      const std::string& hoTen,
                      const std::string& soDienThoai,
                      const std::string& email);

    KhachHang* timTheoId(int id);
    const KhachHang* timTheoId(int id) const;

    // Tìm theo họ tên (so sánh chính xác)
    KhachHang* timTheoHoTen(const std::string& hoTen);
    const KhachHang* timTheoHoTen(const std::string& hoTen) const;

    KhachHang* timTheoSoDienThoai(const std::string& soDienThoai);
    const KhachHang* timTheoSoDienThoai(const std::string& soDienThoai) const;

    // Kiểm tra khách hàng có tồn tại không
    bool tonTai(int id) const;

    void hienThiDanhSach() const;

    const std::vector<KhachHang>& getDanhSach() const;
};
