project(tests)

add_subdirectory(${GTEST_ROOT} gtest) # no idea about second param

add_executable(run_blocking_queue_test blocking_queue_test.cpp )
target_link_libraries(run_blocking_queue_test gtest gtest_main)

add_executable(run_thread_pool_test thread_pool_test.cpp)
target_link_libraries(run_thread_pool_test gtest gtest_main)