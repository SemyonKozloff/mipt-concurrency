#include <thread>

#include "peterson_mutex.h"

peterson_mutex::peterson_mutex()
{
    want_[0].store(false);
    want_[1].store(false);
    victim_.store(0);
}

peterson_mutex::peterson_mutex(peterson_mutex&& mutex)
{
    want_[0].store(mutex.want_[0].load());
    want_[1].store(mutex.want_[1].load());
    victim_.store(mutex.victim_.load());
}

void peterson_mutex::lock(std::size_t thread_id)
{
    want_[thread_id].store(true);
    victim_.store(thread_id);
    while (want_[1 - thread_id].load() && victim_.load() == thread_id)
    {
        std::this_thread::yield();
    }
}

void peterson_mutex::unlock(std::size_t thread_id)
{
    want_[thread_id].store(false);
}
