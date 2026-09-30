#pragma once

#include <string>
#include <vector>
#include "HoaDon.h"
#include "DichVu.h"

class QuanLyHoaDon
{
private:
    std::vector<HoaDon> danhSachHoaDon;

public:
    QuanLyHoaDon() = default;

    void themHoaDon(const HoaDon& hoaDon);

    bool xoaHoaDon(int id);

    HoaDon* timTheoId(int id);
    const HoaDon* timTheoId(int id) const;

    HoaDon* timTheoMaDatSan(int maDatSan);
    const HoaDon* timTheoMaDatSan(int maDatSan) const;

    // Đánh dấu hóa đơn đã thanh toán và lưu ngày thanh toán
    // Trả về true nếu thành công
    bool thanhToan(int id, const std::string& ngayThanhToan);

    void hienThiDanhSach() const;

    // Tính doanh thu các hóa đơn PAID trong ngày cụ thể (định dạng "DD/MM/YYYY")
    double tinhDoanhThuTheoNgay(const std::string& ngay) const;

    // Tính doanh thu các hóa đơn PAID trong tháng/năm cụ thể (thang: 1-12, nam: 4 chữ số)
    double tinhDoanhThuTheoThang(int thang, int nam) const;

    // Tổng doanh thu tất cả hóa đơn PAID
    double tinhTongDoanhThu() const;

    const std::vector<HoaDon>& getDanhSach() const;
};
