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

    std::future<ResultType> submit(std::function<ResultType> function);

    void shutdown();

private:
    constexpr std::size_t default_num_workers_() const noexcept;
    constexpr static std::size_t DEFAULT_NUM_THREADS_ = 3;

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
        for (task current_task;
             !current_task.is_poisoned();
             task_queue_.pop(current_task))
        {
            current_task.execute();
        }
    };

    workers_ = std::move(std::vector(num_threads, std::thread(work)));
}

template<typename ResultType>
std::future<ResultType>
thread_pool<ResultType>::submit(std::function<ResultType> function)
{
    std::promise<ResultType> promise;
    auto future = promise.get_future();

    task new_task(std::move(function), std::move(promise), false);
    task_queue_.push(std::move(new_task));

    return future;
}

template<typename ResultType>
void thread_pool<ResultType>::shutdown()
{
    for (std::size_t i = 0; i < workers_.size(); ++i)
    {
        task poison_pill(true);
        task_queue_.push(std::move(poison_pill)); // push poison pills
    }

    for (auto&& worker : workers_)
    {
        worker.join();
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
    ~task() = default;

    task(const task& task) = delete;
    task& operator=(const task& task) = delete;

    task(task&& task) = default;
    task& operator=(task&& task) = default;

    task() : is_poisoned_(false)
    { }

    explicit task(bool is_poisoned) : is_poisoned_(is_poisoned)
    { }

    task(std::function<ResultType>&& function,
         std::promise<ResultType>&& promise,
         bool is_poisoned) :
            function_(std::move(function)), promise_(std::move(promise)),
            is_poisoned_(is_poisoned)
    { }

    void execute()
    {
        auto result = function_();
        promise_.set_value(std::move(result));
    }

    bool is_poisoned() const noexcept
    {
        return is_poisoned_;
    }

private:
    std::function<ResultType> function_;
    std::promise<ResultType> promise_;

    bool is_poisoned_;
};


#endif //MIPT_CONCURRENCY_THREAD_POOL_H
