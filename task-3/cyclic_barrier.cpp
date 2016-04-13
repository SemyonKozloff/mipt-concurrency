#include "cyclic_barrier.h"

cyclic_barrier::cyclic_barrier(std::size_t num_parties) :
        num_parties_(num_parties), num_parties_on_barrier_(0),
        entrance_is_closed_(false)
{ }

void cyclic_barrier::await()
{
    std::unique_lock<std::mutex> lock(mutex_);
    entrance_gate_cv_.wait(lock, [this] { return !entrance_is_closed_;} );

    if (++num_parties_on_barrier_ == num_parties_)
    {
        entrance_is_closed_ = true;
        exit_gate_cv_.notify_all();
    }
    else
    {
        exit_gate_cv_.wait(lock);
    }

    if (--num_parties_on_barrier_ == 0)
    {
        entrance_is_closed_ = false;
        entrance_gate_cv_.notify_all();
    }

}