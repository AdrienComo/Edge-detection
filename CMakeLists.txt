cmake_minimum_required(VERSION 3.6)
project(ANALYSE_TP2)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS_RELEASE "-OFast")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

set(SOURCE_FILES src/main.cpp src/Image.h src/Image.cpp)
add_executable(ANALYSE_TP2 ${SOURCE_FILES})

target_link_libraries(ANALYSE_TP2 ${OpenCV_LIBS})