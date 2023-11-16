cmake_minimum_required(VERSION 3.26)
project(Server)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

add_executable(Server main.cpp)

include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(Server ${OpenCV_LIBS})


