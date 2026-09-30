#pragma once

#include <string>
#include <vector>
#include "DatSan.h"
#include "QuanLySan.h"
#include "QuanLyKhachHang.h"

/**
 * QuanLyDatSan — Phase 4 + 5 (Booking Logic & Pricing)
 *
 * Manages the booking collection with full validation:
 *  - date/time format and logical correctness
 *  - field existence and active status (via QuanLySan)
 *  - customer existence (via QuanLyKhachHang)
 *  - overlap detection (ignores CANCELLED bookings)
 *
 * Constructor requires references to QuanLySan and QuanLyKhachHang
 * so field/customer lookups stay inside the correct manager.
 */
class QuanLyDatSan
{
private:
    std::vector<DatSan> danhSachDatSan;

    // Non-owning references to peer managers for validation
    QuanLySan&        quanLySan;
    QuanLyKhachHang&  quanLyKhachHang;

    // Internal overlap check (raw — no validation, used after external checks)
    bool coTrungLich(int maSan,
                     const std::string& ngay,
                     const std::string& gioBatDau,
                     const std::string& gioKetThuc,
                     int ngoaiTruId) const;

public:
    QuanLyDatSan(QuanLySan& quanLySan, QuanLyKhachHang& quanLyKhachHang);

    // -----------------------------------------------------------------
    // CRUD
    // -----------------------------------------------------------------

    /**
     * Add a booking after full validation.
     * Returns true only if ALL of the following pass:
     *  1. ngay in YYYY-MM-DD format and logically valid
     *  2. gioBatDau/gioKetThuc in HH:MM and start < end (no midnight crossing)
     *  3. maSan refers to an existing, ACTIVE field
     *  4. maKhachHang refers to an existing customer
     *  5. No overlapping non-cancelled booking on the same field and date
     */
    bool themDatSan(const DatSan& datSan);

    /**
     * Cancel a booking (BOOKED/COMPLETED → CANCELLED).
     * CANCELLED bookings are NOT removed; they remain in the list
     * and are ignored by overlap detection.
     * Returns false if the booking is not found.
     */
    bool huyDatSan(int id);

    /**
     * Edit date/times of an existing booking.
     * Validates the new date/time and re-checks overlap against ALL
     * other bookings (excluding this booking itself).
     * Returns false if not found, invalid data, or overlap detected.
     */
    bool suaDatSan(int id,
                   const std::string& ngay,
                   const std::string& gioBatDau,
                   const std::string& gioKetThuc);

    // -----------------------------------------------------------------
    // SEARCH / LISTING
    // -----------------------------------------------------------------

    DatSan*       timTheoId(int id);
    const DatSan* timTheoId(int id) const;

    std::vector<DatSan*>       layDatSanTheoSan(int maSan);
    std::vector<const DatSan*> layDatSanTheoSan(int maSan) const;

    std::vector<DatSan*>       layDatSanTheoKhachHang(int maKhachHang);
    std::vector<const DatSan*> layDatSanTheoKhachHang(int maKhachHang) const;

    void hienThiDanhSach() const;

    // -----------------------------------------------------------------
    // AVAILABILITY / OVERLAP
    // -----------------------------------------------------------------

    /**
     * kiemTraTrungLich — returns true if the proposed slot conflicts
     * with any existing, non-cancelled booking on the same field and date.
     * ngoaiTruId: booking ID to skip (use when editing an existing booking).
     *
     * NOTE: This does NOT validate the date/time strings.
     *       Callers must validate before calling this function.
     */
    bool kiemTraTrungLich(int maSan,
                          const std::string& ngay,
                          const std::string& gioBatDau,
                          const std::string& gioKetThuc,
                          int ngoaiTruId = -1) const;

    /**
     * kiemTraSanTrong — full availability check.
     * Returns true only when ALL of the following hold:
     *  - field exists and is active
     *  - date string is valid
     *  - time interval is valid (start < end, no midnight crossing)
     *  - no overlapping non-cancelled booking
     */
    bool kiemTraSanTrong(int maSan,
                         const std::string& ngay,
                         const std::string& gioBatDau,
                         const std::string& gioKetThuc,
                         int ngoaiTruId = -1) const;

    const std::vector<DatSan>& getDanhSach() const;

    // -----------------------------------------------------------------
    // PRICING (Phase 5)
    // -----------------------------------------------------------------

    /**
     * tinhTienTheoGio — calculate raw rental cost for a time interval.
     *
     * Pricing schedule (applies to all fields):
     *   00:00–06:00  100,000 VND/hour
     *   06:00–16:00  100,000 VND/hour
     *   16:00–22:00  150,000 VND/hour
     *   22:00–24:00  100,000 VND/hour
     *
     * Splits the interval across pricing boundaries and sums each segment.
     * Supports minute precision (e.g. 15:30–17:30).
     *
     * Returns -1.0 if the time interval is invalid.
     */
    static double tinhTienTheoGio(const std::string& gioBatDau,
                                   const std::string& gioKetThuc);

    /**
     * tinhTienDatSan — calculate the rental cost for a given booking.
     * Returns -1.0 if the booking is CANCELLED or the times are invalid.
     * Does NOT modify the booking object.
     */
    double tinhTienDatSan(const DatSan& datSan) const;

    /**
     * apDungTienDatSan — calculate cost and write it into DatSan.tongTien.
     * Finds the booking by id, calculates, and calls setTongTien.
     * Returns false if the booking is not found or is CANCELLED.
     */
    bool apDungTienDatSan(int id);
};
