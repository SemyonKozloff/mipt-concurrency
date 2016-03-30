#include <future>

#include "gtest/gtest.h"
#include "thread_pool.h"

TEST(thread_pool_test, test_submit)
{
    thread_pool<int> dead_pool;
    auto func = [] { return 42; };
    auto future = dead_pool.submit(func);
    auto result = future.get();

    EXPECT_EQ(42, result);

    dead_pool.shutdown();
}

