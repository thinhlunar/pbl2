#pragma once

#include <string>

/**
 * BoDatSan — pure static helper class for time/date validation.
 * No external dependencies. Used by QuanLyDatSan internally.
 */
class BoDatSan
{
public:
    BoDatSan() = delete; // utility class — no instances

    /**
     * Validate a time string in "HH:MM" format.
     * Valid range: 00:00 – 24:00 (24:00 is allowed as an end-of-day sentinel).
     * Returns false for anything malformed.
     */
    static bool giohopLe(const std::string& gio);

    /**
     * Parse "HH:MM" into total minutes from midnight.
     * Returns -1 if the string is invalid.
     */
    static int phanTichGio(const std::string& gio);

    /**
     * Validate a date string in "YYYY-MM-DD" format.
     * Rejects clearly invalid dates (month > 12, day > days-in-month, etc.).
     * Does NOT reject past dates — that is a business rule for the UI layer.
     */
    static bool ngayHopLe(const std::string& ngay);

    /**
     * Check that a booking interval is logically valid:
     *  - both gioBatDau and gioKetThuc are valid
     *  - gioBatDau < gioKetThuc (duration > 0)
     *  - no midnight crossing (start and end must satisfy start < end numerically)
     */
    static bool khoangGioHopLe(const std::string& gioBatDau,
                                const std::string& gioKetThuc);
};
