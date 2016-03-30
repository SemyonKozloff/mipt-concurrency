cmake_minimum_required(VERSION 3.3)
project(mipt-concurrency)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread -Wall -g")

set(SOURCE_FILES main.cpp
        prac/rw_mutex.h prac/ordered_mutex.h prac/hash_table.h
        task-1/peterson_mutex.h task-1/peterson_mutex.cpp
        task-1/tree_mutex.h task-1/tree_mutex.cpp
        task-2/thread_pool.h task-2/blocking_queue.h
        tests/thread_pool_test.cpp tests/blocking_queue_test.cpp)
add_executable(mipt-concurrency ${SOURCE_FILES})

include_directories(prac)
add_subdirectory(prac)

include_directories(task-1)
add_subdirectory(task-1)

include_directories(task-2)
add_subdirectory(task-2)

set(GTEST_ROOT /home/semyon/Apps/googletest)
include_directories(${GTEST_ROOT} ${GTEST_ROOT}/include)
target_link_libraries(mipt-concurrency gtest gtest_main)
add_subdirectory(tests)







