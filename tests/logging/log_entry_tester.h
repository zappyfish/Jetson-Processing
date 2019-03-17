//
// Created by Liam Kelly on 11/7/18.
//

#ifndef NVIDIA_LOG_ENTRY_TESTER_H
#define NVIDIA_LOG_ENTRY_TESTER_H

#include "test_entry.h"

TEST_CASE("Log Entry Behavior", "[test_entry]") {
    test_entry entry(1.5, 3.3757, 2);

    std::string line = entry.get_log_line();
    size_t ind = 0;

    while(line[ind++] != ',');

    REQUIRE(line.substr(ind) == "a_x,1.500000,a_y,3.375700,a_z,2.000000\n");
}

#endif //NVIDIA_LOG_ENTRY_TESTER_H
