#ifndef MIPT_CONCURRENCY_CYCLIC_BARRIER_H
#define MIPT_CONCURRENCY_CYCLIC_BARRIER_H

#include <condition_variable>

class cyclic_barrier
{
public:
    ~cyclic_barrier() = default;

    cyclic_barrier(const cyclic_barrier& cyclic_barrier) = delete;
    cyclic_barrier& operator=(const cyclic_barrier& cyclic_barrier) = delete;

    cyclic_barrier(cyclic_barrier&& cyclic_barrier) = default;
    cyclic_barrier& operator=(cyclic_barrier&& cyclic_barrier) = default;

    cyclic_barrier() = delete;
    explicit cyclic_barrier(std::size_t num_parties);

    void await();

private:
    const std::size_t num_parties_;
    std::size_t num_parties_on_barrier_;

    std::mutex mutex_;
    std::condition_variable entrance_gate_cv_;
    std::condition_variable exit_gate_cv_;
    bool entrance_is_closed_;
};

#endif //MIPT_CONCURRENCY_CYCLIC_BARRIER_H
