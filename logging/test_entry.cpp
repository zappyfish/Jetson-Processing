//
// Created by Liam Kelly on 11/7/18.
//

#include "test_entry.h"
#include <string>
#include <iostream>

const std::string test_entry::ENTRY_ID = "TEST_ENTRY";

test_entry::test_entry(double a_x, double a_y, double a_z) : log_entry() {
    add_data(std::string("a_x"), a_x);
    add_data(std::string("a_y"), a_y);
    add_data(std::string("a_z"), a_z);
}

std::string test_entry::get_name() { return test_entry::ENTRY_ID; }