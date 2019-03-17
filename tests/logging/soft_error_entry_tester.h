//
// Created by Liam Kelly on 11/8/18.
//

#ifndef NVIDIA_SOFT_ERROR_ENTRY_TESTER_H
#define NVIDIA_SOFT_ERROR_ENTRY_TESTER_H

#include "soft_error_entry.h"
#include <iostream>

TEST_CASE("Soft Error Entry Behavior", "[test_entry]") {
    std::map<std::string, std::string> metadata;
    metadata["test1"] = std::to_string(3.8);
    metadata["test2"] = "hello_world";
    std::string category = "test_error";
    std::string description = "this_is_a_test";
    soft_error_entry s_error(category, description, metadata);

    std::string line = s_error.get_log_line();
    size_t ind = 0;

    while(line[ind++] != ',');

    std::string expected = "category,test_error,description,this_is_a_test,metadata_length,2,test1,3.800000,test2,hello_world\n";
    REQUIRE(expected == line.substr(ind));
}


#endif //NVIDIA_SOFT_ERROR_ENTRY_TESTER_H
