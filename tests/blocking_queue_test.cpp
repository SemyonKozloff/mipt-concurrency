#include <thread>

#include "gtest/gtest.h"
#include "blocking_queue.h"

const std::size_t QUEUE_SIZE = 10;
const std::size_t NUM_TASKS = 10000;
const std::size_t NUM_CONSUMERS = 4;
const int POISON_PILL = -1;

blocking_queue<int> queue(QUEUE_SIZE);

void produce()
{
    for (std::size_t i = 0; i < NUM_TASKS; ++i)
    {
        queue.push(std::rand());
    }
    for (std::size_t i = 0; i < NUM_CONSUMERS; ++i)
    {
        queue.push(POISON_PILL);
    }
}

void consume()
{
    int value = 0;
    while (true)
    {
        queue.pop(value);
        if (value == POISON_PILL)
        {
            break;
        }
        std::size_t hash = std::hash<int>()(value); // some operation
        hash ^= std::rand();
    }
}

TEST(blocking_queue_test, test_push_pop)
{
    std::thread producer(produce);
    std::array<std::thread, NUM_CONSUMERS> consumers;
    for (std::size_t i = 0; i < NUM_CONSUMERS; ++i)
    {
        consumers[i] = std::thread(consume);
    }

    producer.join();
    for (std::size_t i = 0; i < NUM_CONSUMERS; ++i)
    {
        consumers[i].join();
    }
}
