#pragma once

#include <string>
#include <vector>
#include "TaiKhoan.h"

class QuanLyTaiKhoan
{
private:
    std::vector<TaiKhoan> danhSachTaiKhoan;

public:
    QuanLyTaiKhoan() = default;

    // Thêm tài khoản vào danh sách
    void themTaiKhoan(const TaiKhoan& taiKhoan);

    // Xóa tài khoản theo id; trả về true nếu tìm thấy và xóa
    bool xoaTaiKhoan(int id);

    // Trả về con trỏ đến tài khoản tìm được, nullptr nếu không tìm thấy
    TaiKhoan* timTheoId(int id);
    const TaiKhoan* timTheoId(int id) const;

    TaiKhoan* timTheoUsername(const std::string& username);
    const TaiKhoan* timTheoUsername(const std::string& username) const;

    // Kiểm tra username đã tồn tại chưa
    bool usernameTonTai(const std::string& username) const;

    // Đăng nhập: trả về con trỏ đến tài khoản nếu hợp lệ và đang hoạt động, nullptr nếu thất bại
    TaiKhoan* dangNhap(const std::string& username, const std::string& password);

    // Đổi mật khẩu; trả về true nếu thành công
    bool doiMatKhau(int id, const std::string& matKhauMoi);

    // Khóa / mở khóa tài khoản; trả về true nếu tìm thấy
    bool khoaTaiKhoan(int id);
    bool moKhoaTaiKhoan(int id);

    // In danh sách ra std::cout
    void hienThiDanhSach() const;

    const std::vector<TaiKhoan>& getDanhSach() const;
};
