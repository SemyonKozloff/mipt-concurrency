#include <thread>
#include <vector>
#include <mutex>
#include <chrono>

#include "gtest/gtest.h"
#include "spinlock.h"

const std::size_t NUM_THREADS = 4;
const std::size_t NUM_ITERS = 100000;

template<typename LockType>
void test()
{
    std::size_t value = 0;
    LockType mutex;
    std::vector<std::thread> threads;
    for (std::size_t i = 0; i < NUM_THREADS; ++i)
    {
        threads.emplace_back(
                [&value, &mutex]
                {
                    for (std::size_t j = 0; j < NUM_ITERS; ++j)
                    {
                        std::lock_guard<LockType> guard(mutex);
                        value++;
                    }
                });
    }

    for (auto& t : threads)
    {
        t.join();
    }
}

template<typename LockType>
auto chrono_test()
{
    auto begin = std::chrono::high_resolution_clock::now();
    test<LockType>();
    auto diff = std::chrono::high_resolution_clock::now() - begin;
    return diff.count();
}


TEST(spinlock_test, test_performance)
{
    //std::cout << chrono_test<tatas_spinlock>() << std::endl;
    //std::cout << chrono_test<tas_spinlock>() << std::endl;
    //EXPECT_TRUE(chrono_test<tatas_spinlock>() < chrono_test<tas_spinlock>());
}
