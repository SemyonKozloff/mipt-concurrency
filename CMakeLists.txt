cmake_minimum_required(VERSION 3.3)
project(mipt-concurrency)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread -Wall -g")

set(SOURCE_FILES main.cpp
        prac/rw_mutex.h prac/ordered_mutex.h
        task-1/peterson_mutex.h task-1/peterson_mutex.cpp
        task-1/tree_mutex.h task-1/tree_mutex.cpp
        task-2/thread_pool.h tests/thread_pool_test.cpp
        task-2/blocking_queue.h tests/blocking_queue_test.cpp
        task-3/cyclic_barrier.h task-3/cyclic_barrier.cpp
        tests/cyclic_barrier_test.cpp
        task-3/semaphore.cpp task-3/semaphore.h
        task-4/concurrent_hash_set.h tests/concurrent_hash_set_test.cpp
        prac/spinlock.h tests/spinlock_test.cpp
        task-3/robot_sem.h task-3/robot_cond.h tests/robot_test.cpp
        task-6/spsc_ring_buffer.h)
add_executable(mipt-concurrency ${SOURCE_FILES})

include_directories(prac)
add_subdirectory(prac)

include_directories(task-1)
add_subdirectory(task-1)

include_directories(task-2)
add_subdirectory(task-2)

include_directories(task-3)
add_subdirectory(task-3)

include_directories(task-4)
add_subdirectory(task-4)

set(GTEST_ROOT /home/semyon/Apps/googletest)
include_directories(${GTEST_ROOT} ${GTEST_ROOT}/include)
target_link_libraries(mipt-concurrency gtest gtest_main)
add_subdirectory(tests)