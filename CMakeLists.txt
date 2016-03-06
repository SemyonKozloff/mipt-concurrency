cmake_minimum_required(VERSION 3.3)
project(conc_oop)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")

set(SOURCE_FILES main.cpp)
add_executable(conc_oop ${SOURCE_FILES})

add_subdirectory(task1)