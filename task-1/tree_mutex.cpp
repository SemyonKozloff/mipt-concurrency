#include <vector>
#include <atomic>
#include <array>
#include <thread>
#include <algorithm>

class peterson_mutex
{
public:
    peterson_mutex()
    {
        want_[0].store(false);
        want_[1].store(false);
        victim_.store(0);
    }

    peterson_mutex(peterson_mutex&& mutex)
    {
        want_[0].store(mutex.want_[0].load());
        want_[1].store(mutex.want_[1].load());
        victim_.store(mutex.victim_.load());
    }

    void lock(std::size_t thread_id)
    {
        want_[thread_id].store(true);
        victim_.store(thread_id);
        while (want_[1 - thread_id].load() && victim_.load() == thread_id)
        {
            std::this_thread::yield();
        }
    }

    void unlock(std::size_t thread_id)
    {
        want_[thread_id].store(false);
    }

private:
    std::array<std::atomic<bool>, 2> want_;
    std::atomic<std::size_t> victim_;
};



class tree_mutex
{
public:
    tree_mutex(std::size_t num_threads)
    {
        tree_.reserve(2 * num_threads);
        tree_.emplace_back(); // to simplify heap indices
        std::size_t row_size = 1;

        while (row_size < 2 * num_threads)
        {
            for (std::size_t i = 0; i < row_size; ++i)
            {
                tree_.emplace_back();
            }
            row_size *= 2;
        }

        leaves_begin_index_ = tree_.size() - row_size / 2;
        tree_.shrink_to_fit();
    }

    void lock(std::size_t thread_id)
    {
        for (std::size_t current_node_index = leaves_begin_index_ + thread_id;
             current_node_index > ROOT_INDEX_;
             current_node_index = parent_(current_node_index))
        {
            tree_[parent_(current_node_index)].lock(current_node_index % 2);
        }

    }

    void unlock(std::size_t thread_id)
    {
        std::vector<std::size_t> path;

        for (std::size_t current_node_index = leaves_begin_index_ + thread_id;
             current_node_index > ROOT_INDEX_;
             current_node_index = parent_(current_node_index))
        {
            path.push_back(current_node_index);
        }
        std::reverse(std::begin(path), std::end(path));

        for (const auto& current_node_index : path)
        {
            tree_[parent_(current_node_index)].unlock(current_node_index % 2);
        }
    }

private:
    std::size_t parent_(std::size_t i) const noexcept
    {
        return std::size_t(i / 2);
    }

    std::vector<peterson_mutex> tree_;
    std::size_t leaves_begin_index_;

    constexpr static std::size_t ROOT_INDEX_ = 1;
};

