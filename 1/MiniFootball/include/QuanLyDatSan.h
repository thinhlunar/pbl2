#pragma once

#include <string>
#include <vector>
#include "DatSan.h"

class QuanLyDatSan
{
private:
    std::vector<DatSan> danhSachDatSan;

    // Helper: so sánh hai chuỗi giờ "HH:MM" để hỗ trợ kiểm tra trùng lịch
    // Trả về true nếu a < b (theo thứ tự từ điển đối với "HH:MM" là đủ chính xác)
    static bool gioNhoHon(const std::string& a, const std::string& b);

public:
    QuanLyDatSan() = default;

    void themDatSan(const DatSan& datSan);

    // Hủy đặt sân (chuyển trangThai sang "CANCELLED"); trả về true nếu thành công
    bool huyDatSan(int id);

    // Cập nhật ngày/giờ của lịch đặt; trả về true nếu thành công
    bool suaDatSan(int id,
                   const std::string& ngay,
                   const std::string& gioBatDau,
                   const std::string& gioKetThuc);

    DatSan* timTheoId(int id);
    const DatSan* timTheoId(int id) const;

    // Lấy tất cả lịch đặt của một sân
    std::vector<DatSan*> layDatSanTheoSan(int maSan);
    std::vector<const DatSan*> layDatSanTheoSan(int maSan) const;

    // Lấy tất cả lịch đặt của một khách hàng
    std::vector<DatSan*> layDatSanTheoKhachHang(int maKhachHang);
    std::vector<const DatSan*> layDatSanTheoKhachHang(int maKhachHang) const;

    void hienThiDanhSach() const;

    // Kiểm tra một khoảng giờ có trùng lịch với lịch đã đặt không
    // (bỏ qua các lịch CANCELLED và bỏ qua lịch có id == ngoaiTruId nếu >= 0)
    bool kiemTraTrungLich(int maSan,
                          const std::string& ngay,
                          const std::string& gioBatDau,
                          const std::string& gioKetThuc,
                          int ngoaiTruId = -1) const;

    // Trả về true nếu sân trống trong khoảng giờ đó (nghịch đảo kiemTraTrungLich)
    bool kiemTraSanTrong(int maSan,
                         const std::string& ngay,
                         const std::string& gioBatDau,
                         const std::string& gioKetThuc,
                         int ngoaiTruId = -1) const;

    const std::vector<DatSan>& getDanhSach() const;
};
