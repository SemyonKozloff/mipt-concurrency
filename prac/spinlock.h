#ifndef MIPT_CONCURRENCY_SPINLOCK_H
#define MIPT_CONCURRENCY_SPINLOCK_H

#include <atomic>
#include <thread>

constexpr std::size_t CACHE_LINE_SIZE = 64;

/*
 * В некоторых архитектурах с реализацией CAS через LL/SC при попадании
 * двух спинлоков в одну кеш-линию может возникать ситуция ливлока.
 *
 * Чтобы избежать попадания спинлоков в одную линию кеша, нужно использовать
 * выравнивание размера структуры до размера одной кеш-линии.
 */

// alignas не работает ?
class tas_spinlock
{
public:
    void lock()
    {
        while (flag_.exchange(true, std::memory_order_acquire))
        { }
    }

    void unlock()
    {
        flag_.store(false, std::memory_order_release);
    }

private:
    std::atomic_bool flag_;
};

class tatas_spinlock
{
public:
    void lock()
    {
        while (true)
        {
            while (flag_.load(std::memory_order_acquire)) { }
            if (!flag_.exchange(true, std::memory_order_acquire))
            {
                return;
            }
        }
    }

    void unlock()
    {
        flag_.store(false, std::memory_order_release);
    }

private:
    std::atomic_bool flag_;
};

#endif //MIPT_CONCURRENCY_SPINLOCK_H
