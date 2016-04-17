#ifndef MIPT_CONCURRENCY_SPINLOCK_H
#define MIPT_CONCURRENCY_SPINLOCK_H

#include <atomic>
#include <thread>

class tas_spinlock
{
public:
    void lock()
    {
        while (flag_.test_and_set())
        { }
    }

    void unlock()
    {
        flag_.clear();
    }

private:
    std::atomic_flag flag_;
};

class tatas_spinlock
{
public:
    void lock()
    {
        while (true)
        {
            while (flag_.load()) { }
            if (!flag_.exchange(true))
            {
                return;
            }
        }
    }

    void unlock()
    {
        flag_.store(false);
    }

private:
    std::atomic_bool flag_;
};

#endif //MIPT_CONCURRENCY_SPINLOCK_H
