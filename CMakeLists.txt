cmake_minimum_required(VERSION 3.26)
project(Server)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

add_executable(Server main.cpp)

include_directories(${OpenCV_INCLUDE_DIRS} ${GTK3_INCLUDE_DIRS})

target_link_libraries(Server ${OpenCV_LIBS} ${GTK3_LIBRARIES} ssl crypto)
