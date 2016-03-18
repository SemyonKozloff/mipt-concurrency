#ifndef MIPT_CONCURRENCY_ORDERED_MUTEX_H
#define MIPT_CONCURRENCY_ORDERED_MUTEX_H

#include <mutex>
#include <vector>
#include <algorithm>

class ordered_mutex
{
public:
    explicit ordered_mutex(std::size_t number) : number_(number)
    { }

    void lock()
    {
        if (!max_number_stack_.empty() && number_ <= max_number_stack_.back())
        {
            throw std::invalid_argument("Error.");
        }
        max_number_stack_.push_back(number_);
        mutex_.lock();
    }

    void unlock()
    {
        max_number_stack_.erase(std::find(std::begin(max_number_stack_),
                                          std::end(max_number_stack_),
                                          number_));
        mutex_.unlock();
    }

private:
    std::mutex mutex_;
    const std::size_t number_;
    static thread_local std::vector<std::size_t> max_number_stack_;
};

#endif //MIPT_CONCURRENCY_ORDERED_MUTEX_H
