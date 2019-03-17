//
// Created by Liam Kelly on 11/9/18.
//

#ifndef NVIDIA_TIME_MANAGER_TESTER_H
#define NVIDIA_TIME_MANAGER_TESTER_H

#endif //NVIDIA_TIME_MANAGER_TESTER_H

#include "time_manager.h"
#include <chrono>
#include <thread>
#include <string>

/**
 * Note that this test is non-deterministic (yikes) but hopefully it passes every time. Here we want to
 * make sure that the time manager is counting up from zero, and then counts accurately in milliseconds.
 */
TEST_CASE("Time Manager Returns Correct Time", "[time_manager]") {
    time_manager::get_instance().reset_clock();
    int time = std::stoi(time_manager::get_instance().get_relative_time(), nullptr);
    bool condition = (time == 0 || time == 1); // Weird that we have to do this but Catch won't let me put expr in REQUIRE
    REQUIRE(condition);

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    time = std::stoi(time_manager::get_instance().get_relative_time(), nullptr);
    condition = (time >= 50 && time <= 56);
    REQUIRE(condition);
}