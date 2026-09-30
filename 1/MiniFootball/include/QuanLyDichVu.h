#pragma once

#include <string>
#include <vector>
#include "DichVu.h"

/**
 * QuanLyDichVu — Phase 6 (Services & Invoices)
 *
 * Manages the service catalogue with validation:
 *  - ID uniqueness on add
 *  - Name must not be empty
 *  - Price must be >= 0
 *
 * Deletion policy:
 *   xoaDichVu physically removes the service from the catalogue.
 *   Existing ChiTietDichVu records inside invoices keep their maDichVu
 *   but the associated DichVu can no longer be looked up for future
 *   price recalculations. This is intentional: historical invoice lines
 *   are preserved as-is; only future changes would fail silently for the
 *   deleted service. If stronger historical preservation is required,
 *   a "soft delete" flag should be added in a later phase.
 */
class QuanLyDichVu
{
private:
    std::vector<DichVu> danhSachDichVu;

public:
    QuanLyDichVu() = default;

    /**
     * Add a service.
     * Rejects if: ID already exists, name is empty, or price < 0.
     * Returns true on success.
     */
    bool themDichVu(const DichVu& dichVu);

    /**
     * Delete a service by ID.
     * Returns false if not found.
     * See deletion policy in class comment above.
     */
    bool xoaDichVu(int id);

    /**
     * Update name and price of an existing service.
     * Rejects if: not found, new name is empty, or new price < 0.
     */
    bool suaDichVu(int id,
                   const std::string& tenDichVu,
                   double donGia);

    DichVu*       timTheoId(int id);
    const DichVu* timTheoId(int id) const;

    DichVu*       timTheoTen(const std::string& tenDichVu);
    const DichVu* timTheoTen(const std::string& tenDichVu) const;

    void hienThiDanhSach() const;

    const std::vector<DichVu>& getDanhSach() const;
};
