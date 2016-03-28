#ifndef MIPT_CONCURRENCY_HASH_TABLE_H
#define MIPT_CONCURRENCY_HASH_TABLE_H

/*
#include <array>
#include <vector>
#include <forward_list>
#include <iterator>
#include <mutex>
#include <shared_mutex>
#include <atomic>

template<typename T,
        typename HashFunction = std::hash<T>,
        typename EqualityPredicate = std::equal_to<T>>
class hash_table
{
public:
    void insert(const T& element)
    {
        std::size_t hash = HashFunction()(element);
        std::size_t lock_index = hash % locks_.size();
        std::unique_lock<std::shared_timed_mutex> locker(locks_[lock_index]);

        std::size_t bucket_index = hash % table_.size();

        for (auto iter = std::cbegin(table_[bucket_index]);
             iter != std::cend(table_[bucket_index]);
             ++iter)
        {
            if (EqualityPredicate(*iter, element))
            {
                return;
            }
        }

        table_[bucket_index].push_front(element);

        auto new_size = ++size_;
        if (new_size / table_.size() > policy_)
        {
            std::size_t old_table_size = table_.size();
            locker.unlock();
            rehash(old_table_size);
        }
    }

    void rehash(std::size_t old_table_size);


private:
    std::atomic_size_t size_;
    const double policy_;
    std::vector<std::forward_list<T>> table_;
    std::vector<std::shared_timed_mutex> locks_;
};


}
*/

#endif //MIPT_CONCURRENCY_HASH_TABLE_H
