/**
 * main.cpp — Phase 4 + 5 + 6 test driver
 *
 * Phase 4: time/date validation + booking overlap
 * Phase 5: pricing
 * Phase 6: services and invoices
 *
 * Will be replaced by the Raylib UI in a later phase.
 */

#include <cmath>
#include <iostream>
#include <string>

#include "BoDatSan.h"
#include "DatSan.h"
#include "DichVu.h"
#include "HoaDon.h"
#include "KhachHang.h"
#include "QuanLyDatSan.h"
#include "QuanLyDichVu.h"
#include "QuanLyHoaDon.h"
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
    if (condition) { std::cout << "  [PASS] " << label << "\n"; ++passCount; }
    else           { std::cout << "  [FAIL] " << label << "\n"; ++failCount; }
}

static bool nearlyEqual(double a, double b, double eps = 0.01)
{
    return std::fabs(a - b) <= eps;
}

static void checkVal(double actual, double expected, const std::string& label)
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
// Phase 4: validation
// ---------------------------------------------------------------------------
static void testValidation()
{
    std::cout << "\n--- Phase 4: Time/Date validation ---\n";
    check( BoDatSan::giohopLe("06:00"),              "06:00 valid");
    check( BoDatSan::giohopLe("24:00"),              "24:00 valid");
    check(!BoDatSan::giohopLe("25:00"),              "25:00 invalid");
    check(!BoDatSan::giohopLe("18:60"),              "18:60 invalid");
    check(!BoDatSan::giohopLe("abc"),                "abc invalid");
    check(!BoDatSan::khoangGioHopLe("10:00","10:00"),"same time invalid");
    check(!BoDatSan::khoangGioHopLe("23:00","01:00"),"midnight crossing invalid");
    check( BoDatSan::ngayHopLe("2026-09-30"),        "2026-09-30 valid");
    check( BoDatSan::ngayHopLe("2024-02-29"),        "2024-02-29 valid (leap)");
    check(!BoDatSan::ngayHopLe("2026-13-01"),        "2026-13-01 invalid");
    check(!BoDatSan::ngayHopLe("2025-02-29"),        "2025-02-29 invalid (non-leap)");
}

// ---------------------------------------------------------------------------
// Phase 4: booking logic
// ---------------------------------------------------------------------------
static void testBooking()
{
    std::cout << "\n--- Phase 4: Booking logic ---\n";

    QuanLySan       qlSan;
    QuanLyKhachHang qlKH;
    QuanLyDatSan    qlDS(qlSan, qlKH);

    qlSan.themSan(SanBong(1, "San A", "5 nguoi", 100000.0));
    qlKH.themKhachHang(KhachHang(1, "Nguyen Van A", "0901234567", "a@mail.com"));

    DatSan ds1(1, 1, 1, "2026-10-01", "18:00", "20:00");
    check( qlDS.themDatSan(ds1),                                          "Add 18:00-20:00");
    check(!qlDS.themDatSan(DatSan(2,1,1,"2026-10-01","17:00","19:00")),   "17:00-19:00 conflicts");
    check(!qlDS.themDatSan(DatSan(3,1,1,"2026-10-01","19:00","21:00")),   "19:00-21:00 conflicts");
    check( qlDS.themDatSan(DatSan(4,1,1,"2026-10-01","16:00","18:00")),   "16:00-18:00 allowed");
    check( qlDS.themDatSan(DatSan(5,1,1,"2026-10-01","20:00","22:00")),   "20:00-22:00 allowed");
    check(!qlDS.themDatSan(DatSan(6,1,1,"2026-10-05","10:00","10:00")),   "Zero-length rejected");
    check(!qlDS.themDatSan(DatSan(7,99,1,"2026-10-05","08:00","10:00")),  "Non-existent field rejected");
    check( qlDS.huyDatSan(1),                                             "Cancel booking 1");
    check( qlDS.themDatSan(DatSan(8,1,1,"2026-10-01","18:00","20:00")),   "Re-add after cancel");
}

// ---------------------------------------------------------------------------
// Phase 5: pricing
// ---------------------------------------------------------------------------
static void testPricing()
{
    std::cout << "\n--- Phase 5: Pricing ---\n";
    checkVal(QuanLyDatSan::tinhTienTheoGio("10:00","13:00"),  300000.0, "10:00-13:00");
    checkVal(QuanLyDatSan::tinhTienTheoGio("18:00","20:00"),  300000.0, "18:00-20:00");
    checkVal(QuanLyDatSan::tinhTienTheoGio("15:00","17:00"),  250000.0, "15:00-17:00");
    checkVal(QuanLyDatSan::tinhTienTheoGio("21:00","23:00"),  250000.0, "21:00-23:00");
    checkVal(QuanLyDatSan::tinhTienTheoGio("15:00","23:00"), 1100000.0, "15:00-23:00");
    checkVal(QuanLyDatSan::tinhTienTheoGio("15:30","17:30"),  275000.0, "15:30-17:30");
    checkVal(QuanLyDatSan::tinhTienTheoGio("18:30","19:45"),  187500.0, "18:30-19:45");
    checkVal(QuanLyDatSan::tinhTienTheoGio("22:00","24:00"),  200000.0, "22:00-24:00");
    checkVal(QuanLyDatSan::tinhTienTheoGio("abc","10:00"),      -1.0,   "bad start -> -1");
    checkVal(QuanLyDatSan::tinhTienTheoGio("23:00","01:00"),    -1.0,   "midnight -> -1");
}

// ---------------------------------------------------------------------------
// Phase 6: services & invoices
// ---------------------------------------------------------------------------
static void testInvoice()
{
    std::cout << "\n--- Phase 6: Services & Invoices ---\n";

    // Set up infrastructure
    QuanLySan       qlSan;
    QuanLyKhachHang qlKH;
    QuanLyDatSan    qlDS(qlSan, qlKH);
    QuanLyDichVu    qlDV;
    QuanLyHoaDon    qlHD(qlDV, qlDS);

    qlSan.themSan(SanBong(1, "San A", "5 nguoi", 100000.0));
    qlKH.themKhachHang(KhachHang(1, "Nguyen Van A", "0901234567", "a@mail.com"));

    // 15:00-17:00 = 250000 (1h×100k + 1h×150k)
    DatSan ds(1, 1, 1, "2026-10-01", "15:00", "17:00");
    qlDS.themDatSan(ds);
    qlDS.apDungTienDatSan(1);   // sets tongTien = 250000

    // --- Services ---
    check(qlDV.themDichVu(DichVu(1, "Nuoc uong", 10000.0)),  "Add 'Nuoc uong' service");
    check(qlDV.themDichVu(DichVu(2, "Bong da",   50000.0)),  "Add 'Bong da' service");

    // Duplicate ID rejected
    check(!qlDV.themDichVu(DichVu(1, "Duplicate", 5000.0)),  "Duplicate service ID rejected");
    // Empty name rejected
    check(!qlDV.themDichVu(DichVu(3, "",           5000.0)),  "Empty name rejected");
    // Negative price rejected
    check(!qlDV.themDichVu(DichVu(4, "Bad price", -1.0)),     "Negative price rejected");

    // ----- Test 1: Create invoice -----
    check(qlHD.taoHoaDon(1, 1),   "Create invoice for booking 1");
    {
        const HoaDon* hd = qlHD.timTheoId(1);
        checkVal(hd->getTienSan(),   250000.0, "T1 tienSan = 250000");
        checkVal(hd->getTienDichVu(),     0.0, "T1 tienDichVu = 0");
        checkVal(hd->getTongTien(),  250000.0, "T1 tongTien = 250000");
        check(hd->getTrangThai() == "UNPAID", "T1 status = UNPAID");
        check(hd->getNgayThanhToan().empty(), "T1 ngayThanhToan empty");
    }

    // Duplicate invoice rejected
    check(!qlHD.taoHoaDon(2, 1), "Duplicate invoice for same booking rejected");

    // ----- Test 2: Add water ×2 -----
    check(qlHD.themDichVuVaoHoaDon(1, 1, 2), "Add nuoc uong x2");
    {
        const HoaDon* hd = qlHD.timTheoId(1);
        checkVal(hd->getTienDichVu(), 20000.0, "T2 tienDichVu = 20000");
        checkVal(hd->getTongTien(),  270000.0, "T2 tongTien = 270000");
    }

    // ----- Test 3: Add ball ×1 -----
    check(qlHD.themDichVuVaoHoaDon(1, 2, 1), "Add bong da x1");
    {
        const HoaDon* hd = qlHD.timTheoId(1);
        checkVal(hd->getTienDichVu(), 70000.0, "T3 tienDichVu = 70000");
        checkVal(hd->getTongTien(),  320000.0, "T3 tongTien = 320000");
    }

    // ----- Test 4: Increase water from 2 to 3 (add 1 more) -----
    // capNhatSoLuongDichVu sets absolute quantity
    check(qlHD.capNhatSoLuongDichVu(1, 1, 3), "Set nuoc uong qty to 3");
    {
        const HoaDon* hd = qlHD.timTheoId(1);
        // 3×10000 + 1×50000 = 80000
        checkVal(hd->getTienDichVu(), 80000.0, "T4 tienDichVu = 80000");
        checkVal(hd->getTongTien(),  330000.0, "T4 tongTien = 330000");
    }

    // ----- Test 5: Remove ball -----
    check(qlHD.xoaDichVuKhoiHoaDon(1, 2), "Remove bong da");
    {
        const HoaDon* hd = qlHD.timTheoId(1);
        // 3×10000 = 30000
        checkVal(hd->getTienDichVu(), 30000.0, "T5 tienDichVu = 30000");
        checkVal(hd->getTongTien(),  280000.0, "T5 tongTien = 280000");
    }

    // ----- Test 6: Pay invoice -----
    check(qlHD.thanhToan(1, "01/10/2026"), "Pay invoice 1");
    {
        const HoaDon* hd = qlHD.timTheoId(1);
        check(hd->getTrangThai() == "PAID",          "T6 status = PAID");
        check(hd->getNgayThanhToan() == "01/10/2026","T6 ngayThanhToan stored");
    }

    // ----- Test 7: Pay again -> rejected -----
    check(!qlHD.thanhToan(1, "02/10/2026"), "T7 Double payment rejected");

    // ----- Test 8: Revenue counts only PAID -----
    // Create a second booking + invoice but don't pay it
    DatSan ds2(2, 1, 1, "2026-10-02", "18:00", "20:00");
    qlDS.themDatSan(ds2);
    qlDS.apDungTienDatSan(2); // 2h × 150000 = 300000
    qlHD.taoHoaDon(2, 2);
    // invoice 2 is UNPAID

    checkVal(qlHD.tinhTongDoanhThu(),                          280000.0, "T8 total revenue = 280000 (only paid)");
    checkVal(qlHD.tinhDoanhThuTheoNgay("01/10/2026"),          280000.0, "T8 revenue by day 01/10/2026 = 280000");
    checkVal(qlHD.tinhDoanhThuTheoNgay("02/10/2026"),              0.0,  "T8 revenue by day 02/10/2026 = 0 (unpaid)");
    checkVal(qlHD.tinhDoanhThuTheoThang(10, 2026),             280000.0, "T8 revenue Oct 2026 = 280000");

    // Listing helpers
    check(qlHD.layHoaDonDaThanhToan().size()   == 1, "T8 1 PAID invoice");
    check(qlHD.layHoaDonChuaThanhToan().size() == 1, "T8 1 UNPAID invoice");

    // ----- Test 9: Non-existent service -----
    check(!qlHD.themDichVuVaoHoaDon(1, 99, 1), "T9 Non-existent service rejected");
    check(!qlHD.themDichVuVaoHoaDon(99, 1,  1),"T9 Non-existent invoice rejected");

    // ----- Test 10: Invalid quantity -----
    check(!qlHD.themDichVuVaoHoaDon(2, 1, 0),  "T10 qty=0 rejected");
    check(!qlHD.themDichVuVaoHoaDon(2, 1, -1), "T10 qty=-1 rejected");
    check(!qlHD.capNhatSoLuongDichVu(2, 1, 0), "T10 update qty=0 rejected");

    // ----- Cancelled booking cannot get invoice -----
    DatSan ds3(3, 1, 1, "2026-10-03", "08:00", "10:00");
    qlDS.themDatSan(ds3);
    qlDS.apDungTienDatSan(3);
    qlDS.huyDatSan(3);
    check(!qlHD.taoHoaDon(3, 3), "Invoice for CANCELLED booking rejected");

    // ----- QuanLyDichVu: suaDichVu validation -----
    check( qlDV.suaDichVu(1, "Nuoc suoi", 12000.0), "suaDichVu valid update");
    check(!qlDV.suaDichVu(1, "",           12000.0), "suaDichVu empty name rejected");
    check(!qlDV.suaDichVu(1, "Nuoc",        -1.0),  "suaDichVu negative price rejected");
    check(!qlDV.suaDichVu(99,"X",          100.0),  "suaDichVu non-existent rejected");
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------
int main()
{
    std::cout << "=========================================\n";
    std::cout << " Phase 4+5+6 — Full System Tests\n";
    std::cout << "=========================================\n";

    testValidation();
    testBooking();
    testPricing();
    testInvoice();

    std::cout << "\n=========================================\n";
    std::cout << " Results: " << passCount << " passed, "
              << failCount   << " failed\n";
    std::cout << "=========================================\n";

    return failCount == 0 ? 0 : 1;
}
