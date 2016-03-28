#ifndef MIPT_CONCURRENCY_BLOCKING_QUEUE_H
#define MIPT_CONCURRENCY_BLOCKING_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class blocking_queue
{
public:
    blocking_queue();
    ~blocking_queue() = default;

    blocking_queue(const blocking_queue& queue)= delete;
    blocking_queue& operator=(const blocking_queue& queue) = delete;

    blocking_queue(blocking_queue&& queue) = default;
    blocking_queue& operator=(blocking_queue&& queue) = default;

    explicit blocking_queue(std::size_t capacity);

    void push(const T& item);
    void pop(T& item);

private:
    std::queue<T> queue_;
    const std::size_t capacity_;

    mutable std::mutex mutex_;
    mutable std::condition_variable empty_queue_;
    mutable std::condition_variable filled_queue_;
};

template<typename T>
blocking_queue<T>::blocking_queue() :
        capacity_(std::numeric_limits<std::size_t>::max())
{ }

template<typename T>
blocking_queue<T>::blocking_queue(std::size_t capacity) : capacity_(capacity)
{ }

template<typename T>
void blocking_queue<T>::push(const T& item)
{
    std::unique_lock<std::mutex> lock(mutex_);
    filled_queue_.wait(lock, [this] { return queue_.size() < capacity_; });

    queue_.push(item);

    lock.unlock();
    empty_queue_.notify_one();
}

template<typename T>
void blocking_queue<T>::pop(T& item)
{
    std::unique_lock<std::mutex> lock(mutex_);
    empty_queue_.wait(lock, [this] { return !queue_.empty(); });

    item = queue_.front();
    queue_.pop();

    lock.unlock();
    filled_queue_.notify_one();
}

#endif //MIPT_CONCURRENCY_BLOCKING_QUEUE_H
