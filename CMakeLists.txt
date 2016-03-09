cmake_minimum_required(VERSION 3.3)
project(mipt-concurrency)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread -Wall")

set(SOURCE_FILES main.cpp task-1/tree_mutex.cpp)
add_executable(mipt-concurrency ${SOURCE_FILES})
include_directories(task-1)

set(GTEST_ROOT /home/semyon/Apps/googletest)
include_directories(${GTEST_ROOT} ${GTEST_ROOT}/include)

add_subdirectory(task-1)
add_subdirectory(tests)




