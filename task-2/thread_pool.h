#ifndef MIPT_CONCURRENCY_THREAD_POOL_H
#define MIPT_CONCURRENCY_THREAD_POOL_H

#include <vector>
#include <queue>
#include <thread>
#include <future>
#include <atomic>

#include "blocking_queue.h"

template<typename ResultType>
class thread_pool
{
    class task;

public:
    ~thread_pool();

    thread_pool(const thread_pool& thread_pool) = delete;
    thread_pool& operator=(const thread_pool& thread_pool) = delete;

    thread_pool(thread_pool&& thread_pool) = default;
    thread_pool& operator=(thread_pool&& thread_pool) = default;

    thread_pool();
    explicit thread_pool(std::size_t num_threads);

    std::future<ResultType> submit(std::function<ResultType()> function);

    void shutdown();

private:
    std::size_t default_num_workers_() const noexcept;
    constexpr static std::size_t DEFAULT_NUM_THREADS_ = 4;

    std::vector<std::thread> workers_;
    blocking_queue<task> task_queue_;
};

template<typename ResultType>
thread_pool<ResultType>::~thread_pool()
{
    shutdown();
}

template<typename ResultType>
thread_pool<ResultType>::thread_pool() : thread_pool(default_num_workers_())
{ }

template<typename ResultType>
thread_pool<ResultType>::thread_pool(std::size_t num_threads)
{
    auto work = [this]
    {
        task current_task;
        while (true)
        {
            task_queue_.pop(current_task);
            if (current_task.is_poisoned())
            {
                break;
            }
            current_task.execute();
        }
    };

    for (std::size_t i = 0; i < num_threads; ++i)
    {
        workers_.push_back(std::thread(work));
    }
}

template<typename ResultType>
std::future<ResultType>
thread_pool<ResultType>::submit(std::function<ResultType()> function)
{
    auto promise_ptr = std::make_shared<std::promise<ResultType>>();

    task new_task(function, promise_ptr, false);
    task_queue_.push(new_task);

    return promise_ptr->get_future();
}

template<typename ResultType>
void thread_pool<ResultType>::shutdown()
{
    for (std::size_t i = 0; i < workers_.size(); ++i) // pushing poison pills
    {
        task poison_pill(true);
        task_queue_.push(poison_pill);
    }

    for (auto&& worker : workers_)
    {
        if (worker.joinable())
        {
            worker.join();
        }
    }
}

template<typename ResultType>
std::size_t thread_pool<ResultType>::default_num_workers_() const noexcept
{
    std::size_t num_hardware_threads = std::thread::hardware_concurrency();
    return num_hardware_threads == 0 ?
           DEFAULT_NUM_THREADS_ :
           num_hardware_threads;
}

template<typename ResultType>
class thread_pool<ResultType>::task
{
public:
    task() : is_poisoned_(false)
    { }

    explicit task(bool is_poisoned) : is_poisoned_(is_poisoned)
    { }

    task(std::function<ResultType()> function,
         std::shared_ptr<std::promise<ResultType>> promise_ptr,
         bool is_poisoned) :
            function_(function),
            promise_ptr_(promise_ptr),
            is_poisoned_(is_poisoned)
    { }

    void execute()
    {
        ResultType result;
        try
        {
            result = function_();
            promise_ptr_->set_value(result);
        }
        catch (...)
        {
            promise_ptr_->set_exception(std::current_exception());
        }
    }

    bool is_poisoned() const noexcept
    {
        return is_poisoned_;
    }

private:
    std::function<ResultType()> function_;
    std::shared_ptr<std::promise<ResultType>> promise_ptr_;

    bool is_poisoned_;
};

#endif //MIPT_CONCURRENCY_THREAD_POOL_H
