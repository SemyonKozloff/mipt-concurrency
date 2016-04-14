#ifndef MIPT_CONCURRENCY_CONCURRENT_HASH_SET_H
#define MIPT_CONCURRENCY_CONCURRENT_HASH_SET_H

#include <vector>
#include <forward_list>
#include <shared_mutex>


template<typename ValueType,
        typename HashFunction = std::hash<ValueType>,
        typename EqualityPredicate = std::equal_to<ValueType>>
class concurrent_hash_set
{
public:
    ~concurrent_hash_set() = default;

    concurrent_hash_set(const concurrent_hash_set& hash_set) = delete;
    concurrent_hash_set&
            operator=(const concurrent_hash_set& hash_set) = delete;

    concurrent_hash_set(concurrent_hash_set&& hash_set) = default;
    concurrent_hash_set& operator=(concurrent_hash_set&& hash_set) = default;

    explicit
    concurrent_hash_set(std::size_t num_stripes = DEF_NUM_STRIPES_,
                        std::size_t growth_factor = DEF_GROWTH_FACTOR_,
                        float max_load_factor = DEF_MAX_LOAD_FACTOR_) :
            growth_factor_(growth_factor),
            max_load_factor_(max_load_factor),
            num_elements_(0),
            hash_table_(4 * num_stripes),
            mutex_array_(num_stripes)

    { }

    void insert(const ValueType& new_value)
    {
        std::size_t hash = hash_function_(new_value);
        std::size_t mutex_index = hash % mutex_array_.size();

        std::unique_lock<std::shared_timed_mutex>
                lock(mutex_array_[mutex_index]);

        std::size_t bucket_index = hash % hash_table_.size();
        for (const auto& hashed_value : hash_table_[bucket_index])
        {
            if (equal_(hashed_value, new_value))
            {
                return;
            }
        }

        hash_table_[bucket_index].push_front(new_value);
        ++num_elements_;

        float load_factor = num_elements_ / hash_table_.size();
        std::size_t table_size = hash_table_.size();
        if (load_factor >= max_load_factor_)
        {
            lock.unlock();
            rehash_(table_size);
        }
    }

    void erase(const ValueType& value)
    {
        std::size_t hash = hash_function_(value);
        std::size_t mutex_index = hash % mutex_array_.size();

        std::unique_lock<std::shared_timed_mutex>
                lock(mutex_array_[mutex_index]);

        std::size_t bucket_index = hash % hash_table_.size();
        hash_table_[bucket_index].remove(value);
    }

    bool contains(const ValueType& value) const
    {
        std::size_t hash = hash_function_(value);
        std::size_t mutex_index = hash / mutex_array_.size();

        std::shared_lock<std::shared_timed_mutex>
                lock(mutex_array_[mutex_index]);

        std::size_t bucket_index = hash % hash_table_.size();
        for (const auto& hashed_value : hash_table_[bucket_index])
        {
            if (equal_(hashed_value, value))
            {
                return true;
            }
        }

        return false;
    }

    std::size_t size() const noexcept
    {
        return num_elements_;
    }

    bool empty() const noexcept
    {
        return static_cast<bool>(num_elements_);
    }

private:
    bool rehash_(std::size_t old_table_size)
    {
        std::vector<std::unique_lock<std::shared_timed_mutex>> lock_array;

        lock_array.emplace_back(mutex_array_[0]);
        if (old_table_size != hash_table_.size())
        {
            return false;
        }

        for (std::size_t i = 1; i < mutex_array_.size(); ++i)
        {
            lock_array.emplace_back(mutex_array_[i]);
        }

        std::size_t new_table_size = growth_factor_ * hash_table_.size();
        std::vector<std::forward_list<ValueType>>
        temp_hash_table(new_table_size);

        for (const auto& bucket : hash_table_)
        {
            for (const auto& value : bucket)
            {
                std::size_t hash = hash_function_(value);
                std::size_t bucket_index = hash % temp_hash_table.size();
                temp_hash_table[bucket_index].push_front(value);
            }
        }

        hash_table_ = std::move(temp_hash_table);
        return true;
    }

    HashFunction hash_function_;
    EqualityPredicate equal_;

    const std::size_t growth_factor_;
    const float max_load_factor_;

    std::size_t num_elements_;
    std::vector<std::forward_list<ValueType>> hash_table_;

    mutable std::vector<std::shared_timed_mutex> mutex_array_;

    constexpr static std::size_t DEF_NUM_STRIPES_ = 8;

    constexpr static std::size_t DEF_GROWTH_FACTOR_ = 2;
    constexpr static float DEF_MAX_LOAD_FACTOR_ = 2.0;
};

#endif //MIPT_CONCURRENCY_CONCURRENT_HASH_SET_H

