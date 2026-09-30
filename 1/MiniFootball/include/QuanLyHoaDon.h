#pragma once

#include <string>
#include <vector>
#include "HoaDon.h"
#include "DichVu.h"
#include "DatSan.h"

/**
 * QuanLyHoaDon — Phase 6 (Services & Invoices)
 *
 * Manages invoice creation, service line management, payment, and revenue.
 *
 * Requires a reference to QuanLyDichVu for service lookups.
 * Requires a reference to QuanLyDatSan for booking validation.
 *
 * Design decisions:
 *  - Duplicate invoices for the same DatSan are rejected.
 *  - Invoices for CANCELLED bookings are rejected.
 *  - Payment is only allowed on UNPAID invoices.
 *  - Adding a service that already exists in an invoice increases its quantity.
 *  - Revenue counts only PAID invoices.
 *  - ngayThanhToan uses "DD/MM/YYYY" format for revenue date/month filtering.
 */

// Forward-declared to avoid circular headers; include full headers in .cpp
class QuanLyDichVu;
class QuanLyDatSan;

class QuanLyHoaDon
{
private:
    std::vector<HoaDon> danhSachHoaDon;

    QuanLyDichVu& quanLyDichVu;
    QuanLyDatSan& quanLyDatSan;

    // Recalculate tienDichVu from current ChiTietDichVu lines,
    // then update tongTien = tienSan + tienDichVu.
    void capNhatTong(HoaDon& hd);

public:
    QuanLyHoaDon(QuanLyDichVu& quanLyDichVu, QuanLyDatSan& quanLyDatSan);

    // -----------------------------------------------------------------
    // INVOICE CREATION
    // -----------------------------------------------------------------

    /**
     * Create an invoice from a booking.
     * tienSan is taken from DatSan.tongTien (already calculated in Phase 5).
     *
     * Rejected if:
     *  - booking not found
     *  - booking is CANCELLED
     *  - an invoice for this booking already exists
     *  - invoice ID is already used
     *
     * Returns true on success; the invoice is stored internally.
     */
    bool taoHoaDon(int maHoaDon, int maDatSan);

    // Raw add (no booking validation — for loading from file in Phase 7)
    void themHoaDon(const HoaDon& hoaDon);

    bool xoaHoaDon(int id);

    // -----------------------------------------------------------------
    // SERVICE LINE MANAGEMENT
    // -----------------------------------------------------------------

    /**
     * Add or merge a service into an invoice.
     *  - invoice must exist
     *  - service must exist in QuanLyDichVu
     *  - soLuong must be > 0
     *  - if the service already exists, increase its quantity
     * Recalculates tienDichVu and tongTien.
     */
    bool themDichVuVaoHoaDon(int maHoaDon, int maDichVu, int soLuong);

    /**
     * Remove a service line from an invoice.
     * Recalculates totals.
     * Returns false if invoice or service line not found.
     */
    bool xoaDichVuKhoiHoaDon(int maHoaDon, int maDichVu);

    /**
     * Update quantity of an existing service line.
     * soLuong must be > 0.
     * Recalculates totals.
     */
    bool capNhatSoLuongDichVu(int maHoaDon, int maDichVu, int soLuong);

    // -----------------------------------------------------------------
    // PAYMENT
    // -----------------------------------------------------------------

    /**
     * Mark invoice as PAID.
     *  - invoice must exist
     *  - invoice must currently be UNPAID
     *  - ngayThanhToan must be non-empty
     * Returns false on any violation.
     */
    bool thanhToan(int maHoaDon, const std::string& ngayThanhToan);

    // -----------------------------------------------------------------
    // LOOKUPS / LISTING
    // -----------------------------------------------------------------

    HoaDon*       timTheoId(int id);
    const HoaDon* timTheoId(int id) const;

    HoaDon*       timTheoMaDatSan(int maDatSan);
    const HoaDon* timTheoMaDatSan(int maDatSan) const;

    std::vector<HoaDon*>       layHoaDonChuaThanhToan();
    std::vector<const HoaDon*> layHoaDonChuaThanhToan() const;

    std::vector<HoaDon*>       layHoaDonDaThanhToan();
    std::vector<const HoaDon*> layHoaDonDaThanhToan() const;

    void hienThiDanhSach() const;

    // -----------------------------------------------------------------
    // REVENUE (PAID invoices only)
    // -----------------------------------------------------------------

    // ngay format: "DD/MM/YYYY"
    double tinhDoanhThuTheoNgay(const std::string& ngay) const;

    // thang: 1-12, nam: 4-digit year
    double tinhDoanhThuTheoThang(int thang, int nam) const;

    double tinhTongDoanhThu() const;

    const std::vector<HoaDon>& getDanhSach() const;
};
