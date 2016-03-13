#include <vector>
#include <algorithm>

#include "tree_mutex.h"

tree_mutex::tree_mutex(std::size_t num_threads)
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

void tree_mutex::lock(std::size_t thread_id)
{
    for (std::size_t current_node_index = leaves_begin_index_ + thread_id;
         current_node_index > ROOT_INDEX_;
         current_node_index = parent_(current_node_index))
    {
        tree_[parent_(current_node_index)].lock(current_node_index % 2);
    }
}

void tree_mutex::unlock(std::size_t thread_id)
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

std::size_t tree_mutex::parent_(std::size_t i) const noexcept
{
    return std::size_t(i / 2);
}

