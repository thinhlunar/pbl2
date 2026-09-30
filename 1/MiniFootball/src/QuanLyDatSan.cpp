#include "QuanLyDatSan.h"

#include <algorithm>
#include <iostream>

// So sánh chuỗi giờ "HH:MM" — lexicographic order là đủ với định dạng chuẩn
bool QuanLyDatSan::gioNhoHon(const std::string& a, const std::string& b)
{
    return a < b;
}

void QuanLyDatSan::themDatSan(const DatSan& datSan)
{
    danhSachDatSan.push_back(datSan);
}

bool QuanLyDatSan::huyDatSan(int id)
{
    DatSan* ds = timTheoId(id);
    if (ds == nullptr)
        return false;
    ds->setTrangThai("CANCELLED");
    return true;
}

bool QuanLyDatSan::suaDatSan(int id,
                              const std::string& ngay,
                              const std::string& gioBatDau,
                              const std::string& gioKetThuc)
{
    DatSan* ds = timTheoId(id);
    if (ds == nullptr)
        return false;
    ds->setNgay(ngay);
    ds->setGioBatDau(gioBatDau);
    ds->setGioKetThuc(gioKetThuc);
    return true;
}

DatSan* QuanLyDatSan::timTheoId(int id)
{
    for (DatSan& ds : danhSachDatSan)
        if (ds.getId() == id)
            return &ds;
    return nullptr;
}

const DatSan* QuanLyDatSan::timTheoId(int id) const
{
    for (const DatSan& ds : danhSachDatSan)
        if (ds.getId() == id)
            return &ds;
    return nullptr;
}

std::vector<DatSan*> QuanLyDatSan::layDatSanTheoSan(int maSan)
{
    std::vector<DatSan*> result;
    for (DatSan& ds : danhSachDatSan)
        if (ds.getMaSan() == maSan)
            result.push_back(&ds);
    return result;
}

std::vector<const DatSan*> QuanLyDatSan::layDatSanTheoSan(int maSan) const
{
    std::vector<const DatSan*> result;
    for (const DatSan& ds : danhSachDatSan)
        if (ds.getMaSan() == maSan)
            result.push_back(&ds);
    return result;
}

std::vector<DatSan*> QuanLyDatSan::layDatSanTheoKhachHang(int maKhachHang)
{
    std::vector<DatSan*> result;
    for (DatSan& ds : danhSachDatSan)
        if (ds.getMaKhachHang() == maKhachHang)
            result.push_back(&ds);
    return result;
}

std::vector<const DatSan*> QuanLyDatSan::layDatSanTheoKhachHang(int maKhachHang) const
{
    std::vector<const DatSan*> result;
    for (const DatSan& ds : danhSachDatSan)
        if (ds.getMaKhachHang() == maKhachHang)
            result.push_back(&ds);
    return result;
}

void QuanLyDatSan::hienThiDanhSach() const
{
    std::cout << "=== DANH SACH DAT SAN ===\n";
    for (const DatSan& ds : danhSachDatSan)
    {
        std::cout << "ID: "         << ds.getId()
                  << " | San: "      << ds.getMaSan()
                  << " | KH: "       << ds.getMaKhachHang()
                  << " | Ngay: "     << ds.getNgay()
                  << " | "           << ds.getGioBatDau()
                  << " - "           << ds.getGioKetThuc()
                  << " | Trang thai: " << ds.getTrangThai()
                  << "\n";
    }
}

bool QuanLyDatSan::kiemTraTrungLich(int maSan,
                                     const std::string& ngay,
                                     const std::string& gioBatDau,
                                     const std::string& gioKetThuc,
                                     int ngoaiTruId) const
{
    for (const DatSan& ds : danhSachDatSan)
    {
        // Bỏ qua lịch bị hủy
        if (ds.getTrangThai() == "CANCELLED")
            continue;

        // Bỏ qua lịch được loại trừ (dùng khi sửa)
        if (ngoaiTruId >= 0 && ds.getId() == ngoaiTruId)
            continue;

        // Chỉ so sánh cùng sân và cùng ngày
        if (ds.getMaSan() != maSan || ds.getNgay() != ngay)
            continue;

        // Hai khoảng [start1, end1) và [start2, end2) trùng khi: start1 < end2 AND start2 < end1
        const std::string& s1 = gioBatDau;
        const std::string& e1 = gioKetThuc;
        const std::string& s2 = ds.getGioBatDau();
        const std::string& e2 = ds.getGioKetThuc();

        if (gioNhoHon(s1, e2) && gioNhoHon(s2, e1))
            return true;
    }
    return false;
}

bool QuanLyDatSan::kiemTraSanTrong(int maSan,
                                    const std::string& ngay,
                                    const std::string& gioBatDau,
                                    const std::string& gioKetThuc,
                                    int ngoaiTruId) const
{
    return !kiemTraTrungLich(maSan, ngay, gioBatDau, gioKetThuc, ngoaiTruId);
}

const std::vector<DatSan>& QuanLyDatSan::getDanhSach() const
{
    return danhSachDatSan;
}
