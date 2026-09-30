cmake_minimum_required(VERSION 3.20)

project(MiniFootball)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(RAYLIB REQUIRED raylib)

add_executable(MiniFootball
    main.cpp
    src/TaiKhoan.cpp
    src/KhachHang.cpp
    src/SanBong.cpp
    src/DatSan.cpp
    src/DichVu.cpp
    src/ChiTietDichVu.cpp
    src/HoaDon.cpp
    src/QuanLyTaiKhoan.cpp
    src/QuanLyKhachHang.cpp
    src/QuanLySan.cpp
    src/QuanLyDatSan.cpp
    src/QuanLyDichVu.cpp
    src/QuanLyHoaDon.cpp
)

target_include_directories(MiniFootball PRIVATE
    include/
    ${RAYLIB_INCLUDE_DIRS}
)

target_link_directories(MiniFootball PRIVATE
    ${RAYLIB_LIBRARY_DIRS}
)

target_link_libraries(MiniFootball
    ${RAYLIB_LIBRARIES}
)
