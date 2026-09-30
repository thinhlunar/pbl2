#include "BoDatSan.h"

#include <cctype>
#include <stdexcept>
#include <string>

// ---------------------------------------------------------------------------
// giohopLe
// ---------------------------------------------------------------------------
bool BoDatSan::giohopLe(const std::string& gio)
{
    // Must be exactly "HH:MM" — 5 characters
    if (gio.size() != 5)
        return false;

    if (gio[2] != ':')
        return false;

    // All other characters must be digits
    for (int i : {0, 1, 3, 4})
        if (!std::isdigit(static_cast<unsigned char>(gio[i])))
            return false;

    int hours   = std::stoi(gio.substr(0, 2));
    int minutes = std::stoi(gio.substr(3, 2));

    // Allow 00:00 – 24:00 (24:00 = midnight sentinel for end-of-day)
    if (hours < 0 || hours > 24)
        return false;
    if (minutes < 0 || minutes > 59)
        return false;
    // 24:xx is only valid when minutes == 0
    if (hours == 24 && minutes != 0)
        return false;

    return true;
}

// ---------------------------------------------------------------------------
// phanTichGio
// ---------------------------------------------------------------------------
int BoDatSan::phanTichGio(const std::string& gio)
{
    if (!giohopLe(gio))
        return -1;

    int hours   = std::stoi(gio.substr(0, 2));
    int minutes = std::stoi(gio.substr(3, 2));
    return hours * 60 + minutes;
}

// ---------------------------------------------------------------------------
// ngayHopLe
// ---------------------------------------------------------------------------
bool BoDatSan::ngayHopLe(const std::string& ngay)
{
    // Expected format: YYYY-MM-DD (length == 10)
    if (ngay.size() != 10)
        return false;

    if (ngay[4] != '-' || ngay[7] != '-')
        return false;

    // All non-separator positions must be digits
    for (int i : {0, 1, 2, 3, 5, 6, 8, 9})
        if (!std::isdigit(static_cast<unsigned char>(ngay[i])))
            return false;

    int year  = 0;
    int month = 0;
    int day   = 0;

    try
    {
        year  = std::stoi(ngay.substr(0, 4));
        month = std::stoi(ngay.substr(5, 2));
        day   = std::stoi(ngay.substr(8, 2));
    }
    catch (...)
    {
        return false;
    }

    if (month < 1 || month > 12)
        return false;
    if (day < 1)
        return false;

    // Days in each month (non-leap year)
    static const int daysInMonth[13] = {
        0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };

    // Leap year check
    bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    int maxDay  = daysInMonth[month];
    if (month == 2 && isLeap)
        maxDay = 29;

    if (day > maxDay)
        return false;

    return true;
}

// ---------------------------------------------------------------------------
// khoangGioHopLe
// ---------------------------------------------------------------------------
bool BoDatSan::khoangGioHopLe(const std::string& gioBatDau,
                                const std::string& gioKetThuc)
{
    int start = phanTichGio(gioBatDau);
    int end   = phanTichGio(gioKetThuc);

    if (start < 0 || end < 0)
        return false;

    // Duration must be strictly positive and no midnight crossing
    return start < end;
}
