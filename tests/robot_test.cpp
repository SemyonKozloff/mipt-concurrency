#include <thread>

#include "gtest/gtest.h"
#include "robot_cond.h"
#include "robot_sem.h"

constexpr std::size_t NUM_STEPS = 10;

template<typename RobotType>
void test()
{
    RobotType robot;
    auto do_steps = [&robot] (std::string step_type)
    {
        for (std::size_t i = 0; i < NUM_STEPS; ++i)
        {
            robot.step(step_type);
        }
    };

    std::thread left_thread(do_steps, "left");
    std::thread right_thread(do_steps, "right");

    left_thread.join();
    right_thread.join();
}

TEST(robot_test, test_robot_cond)
{
    test<robot_cond>();
}

TEST(robot_test, test_robot_sem)
{
    test<robot_sem>();
}
