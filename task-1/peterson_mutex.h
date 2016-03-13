#ifndef MIPT_CONCURRENCY_PETERSON_MUTEX_H
#define MIPT_CONCURRENCY_PETERSON_MUTEX_H

#include <array>
#include <atomic>

class peterson_mutex
{
public:
    peterson_mutex();
    ~peterson_mutex() = default;

    peterson_mutex(const peterson_mutex& mutex) = delete;
    peterson_mutex& operator=(const peterson_mutex& mutex) = delete;

    peterson_mutex(peterson_mutex&& mutex);
    peterson_mutex& operator=(peterson_mutex&& mutex) = default; // ?

    void lock(std::size_t thread_id);
    void unlock(std::size_t thread_id);

private:
    std::array<std::atomic_bool, 2> want_;
    std::atomic<std::size_t> victim_;
};


#endif //MIPT_CONCURRENCY_PETERSON_MUTEX_H
