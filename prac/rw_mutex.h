#ifndef MIPT_CONCURRENCY_RW_MUTEX_H
#define MIPT_CONCURRENCY_RW_MUTEX_H

#include <mutex>
#include <condition_variable>

class rw_mutex
{
public:
    rw_mutex() : num_readers_(0), is_in_write_(false)
    { }

    void lock()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        writer_.wait(lock, [this] { return !is_in_write_; });
        is_in_write_ = true;
        reader_.wait(lock, [this] { return num_readers_ == 0; });
    }

    void unlock()
    {
        std::unique_lock<std::mutex> lock(mutex_);

        is_in_write_ = false;

        lock.unlock();
        reader_.notify_all();
        writer_.notify_one();
    }

    void lock_shared()
    {
        std::unique_lock<std::mutex> lock(mutex_);

        writer_.wait(lock, [this] { return !is_in_write_; });
        ++num_readers_;
    }

    void unlock_shared()
    {
        std::unique_lock<std::mutex> lock(mutex_);

        if (--num_readers_ == 0)
        {
            lock.unlock();
            reader_.notify_one();
        }
    }

private:
    std::mutex mutex_;

    std::condition_variable reader_;
    std::condition_variable writer_;

    std::size_t num_readers_;
    bool is_in_write_;
};

#endif //MIPT_CONCURRENCY_RW_MUTEX_H
