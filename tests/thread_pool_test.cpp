#include "gtest/gtest.h"
#include "thread_pool.h"
/*
TEST(thread_pool_test, test_something)
{
    thread_pool<int> dead_pool;
    std::function<int()> func = [] { return 42; };
    auto future = dead_pool.submit(func);
    ASSERT_EQ(future.get(), 42);
    dead_pool.shutdown();
}
*/