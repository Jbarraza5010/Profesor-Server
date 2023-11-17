cmake_minimum_required(VERSION 3.26)
project(Server)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

add_subdirectory(/home/tomeito/CLionProjects/Profesor-Server/googletest)

add_executable(Server main.cpp)
target_include_directories(Server PRIVATE ${OpenCV_INCLUDE_DIRS} ${GTK3_INCLUDE_DIRS})
target_link_libraries(Server PRIVATE ${OpenCV_LIBS} ${GTK3_LIBRARIES} ssl crypto)

add_executable(MyTests tests/test.cpp)
target_include_directories(MyTests PRIVATE ${OpenCV_INCLUDE_DIRS} ${GTK3_INCLUDE_DIRS})

target_link_libraries(MyTests PRIVATE gtest gtest_main)