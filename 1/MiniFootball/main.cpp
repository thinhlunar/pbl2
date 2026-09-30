/**
 * main.cpp — Phase 4 test driver
 *
 * Tests booking logic: overlap detection, validation, cancellation,
 * inactive field, invalid dates/times, and editing.
 *
 * Phase 3 will replace this with the Raylib application loop.
 */

#include <cassert>
#include <iostream>
#include <string>

#include "BoDatSan.h"
#include "KhachHang.h"
#include "QuanLyDatSan.h"
#include "QuanLyKhachHang.h"
#include "QuanLySan.h"
#include "SanBong.h"

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------
static int passCount = 0;
static int failCount = 0;

static void check(bool condition, const std::string& testName)
{
    if (condition)
    {
        std::cout << "  [PASS] " << testName << "\n";
        ++passCount;
    }
    else
    {
        std::cout << "  [FAIL] " << testName << "\n";
        ++failCount;
    }
}

// ---------------------------------------------------------------------------
// Time validation tests
// ---------------------------------------------------------------------------
static void testGioHopLe()
{
    std::cout << "\n--- Time validation ---\n";
    check(BoDatSan::giohopLe("06:00"),  "06:00 valid");
    check(BoDatSan::giohopLe("09:30"),  "09:30 valid");
    check(BoDatSan::giohopLe("18:00"),  "18:00 valid");
    check(BoDatSan::giohopLe("23:59"),  "23:59 valid");
    check(BoDatSan::giohopLe("00:00"),  "00:00 valid");
    check(BoDatSan::giohopLe("24:00"),  "24:00 valid (end-of-day sentinel)");
    check(!BoDatSan::giohopLe("25:00"), "25:00 invalid");
    check(!BoDatSan::giohopLe("18:60"), "18:60 invalid");
    check(!BoDatSan::giohopLe("abc"),   "abc invalid");
    check(!BoDatSan::giohopLe("18:5"),  "18:5 invalid (wrong length)");
    check(!BoDatSan::giohopLe("1800"),  "1800 invalid (no colon)");
    check(!BoDatSan::giohopLe(""),      "empty string invalid");
    check(!BoDatSan::giohopLe("24:01"), "24:01 invalid");
}

// ---------------------------------------------------------------------------
// Interval validation tests
// ---------------------------------------------------------------------------
static void testKhoangGio()
{
    std::cout << "\n--- Interval validation ---\n";
    check(BoDatSan::khoangGioHopLe("08:00", "10:00"),  "08:00-10:00 valid");
    check(BoDatSan::khoangGioHopLe("18:00", "20:00"),  "18:00-20:00 valid");
    check(!BoDatSan::khoangGioHopLe("10:00", "10:00"), "same start/end invalid");
    check(!BoDatSan::khoangGioHopLe("20:00", "18:00"), "end before start invalid");
    check(!BoDatSan::khoangGioHopLe("23:00", "01:00"), "midnight crossing invalid");
    check(!BoDatSan::khoangGioHopLe("abc",   "10:00"), "bad start invalid");
    check(!BoDatSan::khoangGioHopLe("08:00", "abc"),   "bad end invalid");
}

// ---------------------------------------------------------------------------
// Date validation tests
// ---------------------------------------------------------------------------
static void testNgayHopLe()
{
    std::cout << "\n--- Date validation ---\n";
    check(BoDatSan::ngayHopLe("2026-09-30"),  "2026-09-30 valid");
    check(BoDatSan::ngayHopLe("2024-02-29"),  "2024-02-29 valid (leap year)");
    check(!BoDatSan::ngayHopLe("2026-13-40"), "2026-13-40 invalid");
    check(!BoDatSan::ngayHopLe("2026-02-30"), "2026-02-30 invalid (Feb overflow)");
    check(!BoDatSan::ngayHopLe("2025-02-29"), "2025-02-29 invalid (non-leap)");
    check(!BoDatSan::ngayHopLe("invalid"),    "invalid string");
    check(!BoDatSan::ngayHopLe("2026/09/30"), "wrong separator");
    check(!BoDatSan::ngayHopLe(""),           "empty string");
}

// ---------------------------------------------------------------------------
// Booking logic tests
// ---------------------------------------------------------------------------
static void testDatSan()
{
    std::cout << "\n--- Booking logic ---\n";

    // Set up managers
    QuanLySan        qlSan;
    QuanLyKhachHang  qlKH;
    QuanLyDatSan     qlDS(qlSan, qlKH);

    // Add a field (active by default)
    qlSan.themSan(SanBong(1, "San A", "5 nguoi", 100000.0));

    // Add a customer
    qlKH.themKhachHang(KhachHang(1, "Nguyen Van A", "0901234567", "a@mail.com"));

    // --- Happy path: add a booking 18:00-20:00 ---
    DatSan ds1(1, 1, 1, "2026-10-01", "18:00", "20:00");
    check(qlDS.themDatSan(ds1), "Add 18:00-20:00 succeeds");

    // --- Overlap cases ---
    DatSan ds_ov1(2, 1, 1, "2026-10-01", "17:00", "19:00");
    check(!qlDS.themDatSan(ds_ov1), "17:00-19:00 conflicts -> rejected");

    DatSan ds_ov2(3, 1, 1, "2026-10-01", "18:00", "19:00");
    check(!qlDS.themDatSan(ds_ov2), "18:00-19:00 conflicts -> rejected");

    DatSan ds_ov3(4, 1, 1, "2026-10-01", "19:00", "21:00");
    check(!qlDS.themDatSan(ds_ov3), "19:00-21:00 conflicts -> rejected");

    DatSan ds_ov4(5, 1, 1, "2026-10-01", "18:30", "19:30");
    check(!qlDS.themDatSan(ds_ov4), "18:30-19:30 conflicts -> rejected");

    // --- Adjacent (allowed) ---
    DatSan ds_adj1(6, 1, 1, "2026-10-01", "16:00", "18:00");
    check(qlDS.themDatSan(ds_adj1), "16:00-18:00 allowed (adjacent before)");

    DatSan ds_adj2(7, 1, 1, "2026-10-01", "20:00", "22:00");
    check(qlDS.themDatSan(ds_adj2), "20:00-22:00 allowed (adjacent after)");

    // --- Different day: no conflict ---
    DatSan ds_diffDay(8, 1, 1, "2026-10-02", "18:00", "20:00");
    check(qlDS.themDatSan(ds_diffDay), "Same slot different day -> allowed");

    // --- Invalid date ---
    DatSan ds_badDate(9, 1, 1, "2026-13-40", "08:00", "10:00");
    check(!qlDS.themDatSan(ds_badDate), "Invalid date -> rejected");

    // --- Invalid time ---
    DatSan ds_badTime(10, 1, 1, "2026-10-05", "25:00", "27:00");
    check(!qlDS.themDatSan(ds_badTime), "Invalid time -> rejected");

    // --- Same start and end ---
    DatSan ds_zeroLen(11, 1, 1, "2026-10-05", "10:00", "10:00");
    check(!qlDS.themDatSan(ds_zeroLen), "Zero-length interval -> rejected");

    // --- Non-existent field ---
    DatSan ds_noField(12, 99, 1, "2026-10-05", "08:00", "10:00");
    check(!qlDS.themDatSan(ds_noField), "Non-existent field -> rejected");

    // --- Inactive field ---
    qlSan.capNhatTrangThai(1, false);
    DatSan ds_inactive(13, 1, 1, "2026-10-06", "08:00", "10:00");
    check(!qlDS.themDatSan(ds_inactive), "Inactive field -> rejected");
    qlSan.capNhatTrangThai(1, true); // restore

    // --- Non-existent customer ---
    DatSan ds_noKH(14, 1, 99, "2026-10-06", "08:00", "10:00");
    check(!qlDS.themDatSan(ds_noKH), "Non-existent customer -> rejected");

    // --- Cancel booking: slot should become free again ---
    check(qlDS.huyDatSan(1), "Cancel booking 1");
    DatSan ds_afterCancel(15, 1, 1, "2026-10-01", "18:00", "20:00");
    check(qlDS.themDatSan(ds_afterCancel), "18:00-20:00 allowed after cancellation");

    // --- Edit booking ---
    // ds_adj2 (id=7) is 20:00-22:00 on 2026-10-01
    // Edit to 20:30-22:00 — should succeed (no overlap except with itself)
    check(qlDS.suaDatSan(7, "2026-10-01", "20:30", "22:30"), "Edit own booking -> success");

    // Edit to overlap with ds_adj1 (id=6, 16:00-18:00) -> fail
    check(!qlDS.suaDatSan(7, "2026-10-01", "16:30", "17:30"), "Edit to overlap -> rejected");

    // Edit non-existent booking
    check(!qlDS.suaDatSan(999, "2026-10-01", "08:00", "10:00"), "Edit non-existent -> false");

    // kiemTraSanTrong full check
    check(qlDS.kiemTraSanTrong(1, "2026-10-03", "08:00", "10:00"), "kiemTraSanTrong: free slot -> true");
    check(!qlDS.kiemTraSanTrong(99, "2026-10-03", "08:00", "10:00"), "kiemTraSanTrong: bad field -> false");
    check(!qlDS.kiemTraSanTrong(1, "2026-13-03", "08:00", "10:00"), "kiemTraSanTrong: bad date -> false");
    check(!qlDS.kiemTraSanTrong(1, "2026-10-03", "abc", "10:00"),   "kiemTraSanTrong: bad time -> false");
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------
int main()
{
    std::cout << "==============================\n";
    std::cout << " Phase 4 — Booking Logic Tests\n";
    std::cout << "==============================\n";

    testGioHopLe();
    testKhoangGio();
    testNgayHopLe();
    testDatSan();

    std::cout << "\n==============================\n";
    std::cout << " Results: " << passCount << " passed, "
              << failCount << " failed\n";
    std::cout << "==============================\n";

    return failCount == 0 ? 0 : 1;
}
