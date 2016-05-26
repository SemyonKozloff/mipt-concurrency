#ifndef MIPT_CONCURRENCY_SEMAPHORE_H
#define MIPT_CONCURRENCY_SEMAPHORE_H

#include <condition_variable>
#include <atomic>

class semaphore
{
public:
    ~semaphore() = default;

    semaphore(const semaphore& sem) = delete;
    semaphore& operator=(const semaphore& sem) = delete;

    semaphore(semaphore&& sem) = default;
    semaphore& operator=(semaphore&& sem) = default;

    semaphore() = delete;
    explicit semaphore(std::size_t initial_counter);

    void wait();
    void signal();

private:
    std::size_t counter_;

    mutable std::condition_variable signal_cv_;
    mutable std::mutex mutex_;
};

#endif //MIPT_CONCURRENCY_SEMAPHORE_H
