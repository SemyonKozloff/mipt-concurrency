#ifndef MIPT_CONCURRENCY_ROBOT_COND_H
#define MIPT_CONCURRENCY_ROBOT_COND_H

#include <iostream>
#include <mutex>
#include <condition_variable>

class robot_cond
{
    enum class turn_type {
        LEFT, RIGHT
    };

public:
    robot_cond() : next_turn_(turn_type::LEFT)
    { }

    void step(std::string step_type)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        if (step_type == "left")
        {
            turn_cv_.wait(lock,
                          [this] { return next_turn_ == turn_type::LEFT; });
            std::cout << "left" << std::endl;
            next_turn_ = turn_type::RIGHT;
            lock.unlock();
            turn_cv_.notify_one();
        }
        else if (step_type == "right")
        {
            turn_cv_.wait(lock,
                          [this] { return next_turn_ == turn_type::RIGHT; });
            std::cout << "right" << std::endl;
            next_turn_ = turn_type::LEFT;
            lock.unlock();
            turn_cv_.notify_one();
        }
    }

private:
    turn_type next_turn_;

    std::condition_variable turn_cv_;
    std::mutex mutex_;
};

#endif //MIPT_CONCURRENCY_ROBOT_COND_H
