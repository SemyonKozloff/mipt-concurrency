#ifndef MIPT_CONCURRENCY_ROBOT_SEM_H
#define MIPT_CONCURRENCY_ROBOT_SEM_H

#include "semaphore.h"

class robot_sem
{
public:
    robot_sem() : left_turn_(0), right_turn_(0)
    {
        left_turn_.signal();
    }

    void step(std::string step_type)
    {
        if (step_type == "left")
        {
            left_turn_.wait();
            std::cout << "left" << std::endl;
            right_turn_.signal();
        }
        else if (step_type == "right")
        {
            right_turn_.wait();
            std::cout << "right" << std::endl;
            left_turn_.signal();
        }
    }

private:
    semaphore left_turn_;
    semaphore right_turn_;
};


#endif //MIPT_CONCURRENCY_ROBOT_SEM_H
