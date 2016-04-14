#include <thread>

#include "gtest/gtest.h"
#include "cyclic_barrier.h"

const std::size_t LENGTH = 10;
const std::size_t NUM_ITERS = 14;

// https://github.com/rlipovsky/mipt-parallel-2015/blob/master/cyclic_barrier/rotate_test.cpp
TEST(cyclic_barrier_test, rotate_test)
{
    std::vector<std::size_t> data;
    for (std::size_t i = 0; i < LENGTH; ++i)
    {
        data.push_back(i);
    }

    cyclic_barrier barrier(data.size());

    std::vector<std::thread> threads;
    for (std::size_t i = 0; i < data.size(); ++i)
    {
        threads.emplace_back(
                [i, &data, &barrier]()
                {
                    for (std::size_t k = 0; k < NUM_ITERS; ++k) {
                        std::size_t next = data[(i + 1) % data.size()];
                        barrier.await();
                        data[i] = next;
                        barrier.await();
                    }
                }
        );
    }

    for (auto& t : threads)
    {
        t.join();
    }

    for (std::size_t i = 0; i < data.size(); ++i)
    {
        EXPECT_EQ((i + NUM_ITERS - LENGTH) % data.size(), data[i]);
    }
}
