#ifndef MIPT_CONCURRENCY_SPSC_RING_BUFFER_H
#define MIPT_CONCURRENCY_SPSC_RING_BUFFER_H

#include <atomic>
#include <vector>
#include <thread>

template <typename Value>
class spsc_ring_buffer
{
public:
    spsc_ring_buffer(std::size_t capacity) : bufffer_(capacity + 1),
                                             head_(0), tail_(0),
                                             capacity_(capacity + 1)
    { }

    bool enqueue(const Value& value)
    {
        std::size_t tail = tail_.load(std::memory_order_relaxed);
        std::size_t after_tail = next_(tail_);
        if (after_tail == head_.load(std::memory_order_acquire))
        {
            return false;
        }

        bufffer_[tail] = value;
        tail_.store(after_tail, std::memory_order_release);

        return true;
    }

    bool dequeue(Value& value)
    {

        std::size_t head = head_.load(std::memory_order_relaxed);
        if (head == tail_.load(std::memory_order_acquire))
        {
            return false;
        }

        value = bufffer_[head];
        head_.store(next_(head), std::memory_order_release);

        return true;
    }

private:
    std::vector<Value> bufffer_;
    std::atomic_size_t head_;
    std::atomic_size_t tail_;
    std::size_t capacity_;

    std::size_t next_(size_t current) const
    {
        return (current + 1) % capacity_;
    }
};

#endif //MIPT_CONCURRENCY_SPSC_RING_BUFFER_H
