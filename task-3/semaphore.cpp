#include "semaphore.h"

semaphore::semaphore(std::size_t initial_counter) : counter_(initial_counter)
{ }

void semaphore::wait()
{
    std::unique_lock<std::mutex> lock(mutex_);
    signal_cv_.wait(lock, [this] { return counter_ > 0; });
    --counter_;
}

void semaphore::signal()
{
    std::lock_guard<std::mutex> lock(mutex_);
    ++counter_;
    signal_cv_.notify_all();
}