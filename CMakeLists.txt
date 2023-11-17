cmake_minimum_required(VERSION 3.26)
project(Server)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

# Add the path to the Google Test source directory
add_subdirectory(/home/tomeito/CLionProjects/Profesor-Server/googletest)

# Main Server executable
add_executable(Server main.cpp)  # Include encryption.cpp here
target_include_directories(Server PRIVATE ${OpenCV_INCLUDE_DIRS} ${GTK3_INCLUDE_DIRS})
target_link_libraries(Server PRIVATE ${OpenCV_LIBS} ${GTK3_LIBRARIES} ssl crypto)

# Test executable
add_executable(MyTests tests/test.cpp)  # Include encryption.cpp here
target_include_directories(MyTests PRIVATE ${OpenCV_INCLUDE_DIRS} ${GTK3_INCLUDE_DIRS})

# Link your test executable with Google Test
target_link_libraries(MyTests PRIVATE gtest gtest_main)

# Add any additional source files needed for tests (additional_source_files.cpp, for example)
# add_executable(MyTests tests/test_main.cpp additional_source_files.cpp)

