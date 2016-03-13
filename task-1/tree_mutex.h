#ifndef MIPT_CONCURRENCY_TREE_MUTEX_H
#define MIPT_CONCURRENCY_TREE_MUTEX_H

#include <vector>

#include "peterson_mutex.h"

class tree_mutex
{
public:
    tree_mutex(std::size_t num_threads);

    void lock(std::size_t thread_id);
    void unlock(std::size_t thread_id);

private:
    std::size_t parent_(std::size_t i) const noexcept;

    std::vector<peterson_mutex> tree_;
    std::size_t leaves_begin_index_;

    constexpr static std::size_t ROOT_INDEX_ = 1;
};



#endif //MIPT_CONCURRENCY_TREE_MUTEX_H
