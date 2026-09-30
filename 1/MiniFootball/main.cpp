/**
 * main.cpp — Phase 4 + Phase 5 test driver
 *
 * Tests:
 *  - Time/date validation (Phase 4)
 *  - Booking overlap logic (Phase 4)
 *  - Pricing calculations (Phase 5)
 *
 * Will be replaced by the Raylib application in Phase 6.
 */

#include <cassert>
#include <cmath>
#include <iostream>
#include <string>

#include "BoDatSan.h"
#include "DatSan.h"
#include "KhachHang.h"
#include "QuanLyDatSan.h"
#include "QuanLyKhachHang.h"
#include "QuanLySan.h"
#include "SanBong.h"

// ---------------------------------------------------------------------------
// Test utilities
// ---------------------------------------------------------------------------
static int passCount = 0;
static int failCount = 0;

static void check(bool condition, const std::string& label)
{
    if (condition)
    {
        std::cout << "  [PASS] " << label << "\n";
        ++passCount;
    }
    else
    {
        std::cout << "  [FAIL] " << label << "\n";
        ++failCount;
    }
}

// Floating-point equality with tolerance
static bool nearlyEqual(double a, double b, double eps = 0.01)
{
    return std::fabs(a - b) <= eps;
}

static void checkPrice(double actual, double expected, const std::string& label)
{
    if (nearlyEqual(actual, expected))
    {
        std::cout << "  [PASS] " << label
                  << " (got " << actual << ", expected " << expected << ")\n";
        ++passCount;
    }
    else
    {
        std::cout << "  [FAIL] " << label
                  << " (got " << actual << ", expected " << expected << ")\n";
        ++failCount;
    }
}

// ---------------------------------------------------------------------------
// Phase 4: time/date validation
// ---------------------------------------------------------------------------
static void testGioHopLe()
{
    std::cout << "\n--- Time validation ---\n";
    check( BoDatSan::giohopLe("06:00"),  "06:00 valid");
    check( BoDatSan::giohopLe("23:59"),  "23:59 valid");
    check( BoDatSan::giohopLe("00:00"),  "00:00 valid");
    check( BoDatSan::giohopLe("24:00"),  "24:00 valid (end-of-day sentinel)");
    check(!BoDatSan::giohopLe("25:00"),  "25:00 invalid");
    check(!BoDatSan::giohopLe("18:60"),  "18:60 invalid");
    check(!BoDatSan::giohopLe("abc"),    "abc invalid");
    check(!BoDatSan::giohopLe("18:5"),   "18:5 invalid (length)");
    check(!BoDatSan::giohopLe("24:01"),  "24:01 invalid");
    check(!BoDatSan::giohopLe(""),       "empty string invalid");
}

static void testKhoangGio()
{
    std::cout << "\n--- Interval validation ---\n";
    check( BoDatSan::khoangGioHopLe("08:00", "10:00"),  "08:00-10:00 valid");
    check(!BoDatSan::khoangGioHopLe("10:00", "10:00"),  "same start/end invalid");
    check(!BoDatSan::khoangGioHopLe("20:00", "18:00"),  "end before start invalid");
    check(!BoDatSan::khoangGioHopLe("23:00", "01:00"),  "midnight crossing invalid");
    check(!BoDatSan::khoangGioHopLe("abc",   "10:00"),  "bad start invalid");
}

static void testNgayHopLe()
{
    std::cout << "\n--- Date validation ---\n";
    check( BoDatSan::ngayHopLe("2026-09-30"),  "2026-09-30 valid");
    check( BoDatSan::ngayHopLe("2024-02-29"),  "2024-02-29 valid (leap year)");
    check(!BoDatSan::ngayHopLe("2026-13-40"),  "2026-13-40 invalid");
    check(!BoDatSan::ngayHopLe("2026-02-30"),  "2026-02-30 invalid (Feb overflow)");
    check(!BoDatSan::ngayHopLe("2025-02-29"),  "2025-02-29 invalid (non-leap)");
    check(!BoDatSan::ngayHopLe("invalid"),     "invalid string");
    check(!BoDatSan::ngayHopLe(""),            "empty string");
}

// ---------------------------------------------------------------------------
// Phase 4: booking logic
// ---------------------------------------------------------------------------
static void testDatSan()
{
    std::cout << "\n--- Booking logic ---\n";

    QuanLySan       qlSan;
    QuanLyKhachHang qlKH;
    QuanLyDatSan    qlDS(qlSan, qlKH);

    qlSan.themSan(SanBong(1, "San A", "5 nguoi", 100000.0));
    qlKH.themKhachHang(KhachHang(1, "Nguyen Van A", "0901234567", "a@mail.com"));

    // Happy path
    DatSan ds1(1, 1, 1, "2026-10-01", "18:00", "20:00");
    check(qlDS.themDatSan(ds1), "Add 18:00-20:00 succeeds");

    // Overlaps
    check(!qlDS.themDatSan(DatSan(2, 1, 1, "2026-10-01", "17:00", "19:00")), "17:00-19:00 conflicts");
    check(!qlDS.themDatSan(DatSan(3, 1, 1, "2026-10-01", "18:00", "19:00")), "18:00-19:00 conflicts");
    check(!qlDS.themDatSan(DatSan(4, 1, 1, "2026-10-01", "19:00", "21:00")), "19:00-21:00 conflicts");
    check(!qlDS.themDatSan(DatSan(5, 1, 1, "2026-10-01", "18:30", "19:30")), "18:30-19:30 conflicts");

    // Adjacent (allowed)
    check(qlDS.themDatSan(DatSan(6, 1, 1, "2026-10-01", "16:00", "18:00")), "16:00-18:00 allowed");
    check(qlDS.themDatSan(DatSan(7, 1, 1, "2026-10-01", "20:00", "22:00")), "20:00-22:00 allowed");

    // Different day
    check(qlDS.themDatSan(DatSan(8, 1, 1, "2026-10-02", "18:00", "20:00")), "Same slot different day OK");

    // Bad date/time
    check(!qlDS.themDatSan(DatSan(9,  1, 1, "2026-13-40", "08:00", "10:00")), "Invalid date rejected");
    check(!qlDS.themDatSan(DatSan(10, 1, 1, "2026-10-05", "25:00", "27:00")), "Invalid time rejected");
    check(!qlDS.themDatSan(DatSan(11, 1, 1, "2026-10-05", "10:00", "10:00")), "Zero-length rejected");

    // Bad field / customer
    check(!qlDS.themDatSan(DatSan(12, 99, 1,  "2026-10-05", "08:00", "10:00")), "Non-existent field rejected");
    check(!qlDS.themDatSan(DatSan(13, 1,  99, "2026-10-06", "08:00", "10:00")), "Non-existent customer rejected");

    // Inactive field
    qlSan.capNhatTrangThai(1, false);
    check(!qlDS.themDatSan(DatSan(14, 1, 1, "2026-10-06", "08:00", "10:00")), "Inactive field rejected");
    qlSan.capNhatTrangThai(1, true);

    // Cancel + re-add
    check(qlDS.huyDatSan(1), "Cancel booking 1");
    check(qlDS.themDatSan(DatSan(15, 1, 1, "2026-10-01", "18:00", "20:00")), "Re-add after cancel OK");

    // Edit
    check( qlDS.suaDatSan(7, "2026-10-01", "20:30", "22:30"),           "Edit to non-overlap -> OK");
    check(!qlDS.suaDatSan(7, "2026-10-01", "16:30", "17:30"),           "Edit to overlap -> rejected");
    check(!qlDS.suaDatSan(999, "2026-10-01", "08:00", "10:00"),         "Edit non-existent -> false");

    // kiemTraSanTrong
    check( qlDS.kiemTraSanTrong(1, "2026-10-03", "08:00", "10:00"),     "Free slot -> true");
    check(!qlDS.kiemTraSanTrong(99, "2026-10-03", "08:00", "10:00"),    "Bad field -> false");
    check(!qlDS.kiemTraSanTrong(1, "2026-13-03", "08:00", "10:00"),     "Bad date -> false");
    check(!qlDS.kiemTraSanTrong(1, "2026-10-03", "abc", "10:00"),       "Bad time -> false");
}

// ---------------------------------------------------------------------------
// Phase 5: pricing
// ---------------------------------------------------------------------------
static void testGia()
{
    std::cout << "\n--- Pricing calculations ---\n";

    // Required test cases from the spec
    checkPrice(QuanLyDatSan::tinhTienTheoGio("10:00", "13:00"),  300000.0, "10:00-13:00");
    checkPrice(QuanLyDatSan::tinhTienTheoGio("18:00", "20:00"),  300000.0, "18:00-20:00");
    checkPrice(QuanLyDatSan::tinhTienTheoGio("15:00", "17:00"),  250000.0, "15:00-17:00");
    checkPrice(QuanLyDatSan::tinhTienTheoGio("21:00", "23:00"),  250000.0, "21:00-23:00");
    checkPrice(QuanLyDatSan::tinhTienTheoGio("15:00", "23:00"), 1100000.0, "15:00-23:00");
    checkPrice(QuanLyDatSan::tinhTienTheoGio("15:30", "17:30"),  275000.0, "15:30-17:30");
    checkPrice(QuanLyDatSan::tinhTienTheoGio("18:30", "19:45"),  187500.0, "18:30-19:45");
    checkPrice(QuanLyDatSan::tinhTienTheoGio("22:00", "24:00"),  200000.0, "22:00-24:00");

    // Edge: pure daytime (all in [06:00-16:00])
    checkPrice(QuanLyDatSan::tinhTienTheoGio("06:00", "07:00"),  100000.0, "06:00-07:00 (1h daytime)");

    // Edge: pure peak
    checkPrice(QuanLyDatSan::tinhTienTheoGio("16:00", "17:00"),  150000.0, "16:00-17:00 (1h peak)");

    // Edge: early morning [00:00-06:00]
    checkPrice(QuanLyDatSan::tinhTienTheoGio("00:00", "06:00"),  600000.0, "00:00-06:00 (6h early)");

    // Invalid times -> -1.0
    checkPrice(QuanLyDatSan::tinhTienTheoGio("abc",   "10:00"),  -1.0,  "bad start -> -1.0");
    checkPrice(QuanLyDatSan::tinhTienTheoGio("10:00", "10:00"),  -1.0,  "same time -> -1.0");
    checkPrice(QuanLyDatSan::tinhTienTheoGio("20:00", "18:00"),  -1.0,  "end < start -> -1.0");
    checkPrice(QuanLyDatSan::tinhTienTheoGio("23:00", "01:00"),  -1.0,  "midnight crossing -> -1.0");

    // tinhTienDatSan and apDungTienDatSan
    std::cout << "\n--- tinhTienDatSan / apDungTienDatSan ---\n";

    QuanLySan       qlSan;
    QuanLyKhachHang qlKH;
    QuanLyDatSan    qlDS(qlSan, qlKH);

    qlSan.themSan(SanBong(1, "San A", "5 nguoi", 100000.0));
    qlKH.themKhachHang(KhachHang(1, "Nguyen Van A", "0901234567", "a@mail.com"));

    DatSan ds1(1, 1, 1, "2026-10-01", "15:00", "17:00");
    qlDS.themDatSan(ds1);

    const DatSan* p1 = qlDS.timTheoId(1);
    checkPrice(qlDS.tinhTienDatSan(*p1), 250000.0, "tinhTienDatSan 15:00-17:00");

    check(qlDS.apDungTienDatSan(1),   "apDungTienDatSan(1) returns true");
    const DatSan* p1u = qlDS.timTheoId(1);
    checkPrice(p1u->getTongTien(), 250000.0, "tongTien updated to 250000 after apDung");

    // Cancelled booking
    DatSan ds2(2, 1, 1, "2026-10-02", "18:00", "20:00");
    qlDS.themDatSan(ds2);
    qlDS.huyDatSan(2);
    const DatSan* p2 = qlDS.timTheoId(2);
    checkPrice(qlDS.tinhTienDatSan(*p2), -1.0, "CANCELLED booking -> -1.0");
    check(!qlDS.apDungTienDatSan(2),           "apDungTienDatSan on CANCELLED -> false");

    // Non-existent
    check(!qlDS.apDungTienDatSan(999), "apDungTienDatSan non-existent -> false");
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------
int main()
{
    std::cout << "======================================\n";
    std::cout << " Phase 4+5 — Booking & Pricing Tests\n";
    std::cout << "======================================\n";

    testGioHopLe();
    testKhoangGio();
    testNgayHopLe();
    testDatSan();
    testGia();

    std::cout << "\n======================================\n";
    std::cout << " Results: " << passCount << " passed, "
              << failCount << " failed\n";
    std::cout << "======================================\n";

    return failCount == 0 ? 0 : 1;
}
