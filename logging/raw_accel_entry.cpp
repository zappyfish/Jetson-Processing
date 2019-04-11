//
// Created by Liam Kelly on 4/5/19.
//

#include "raw_accel_entry.h"

const std::string raw_accel_entry::ENTRY_ID = "raw_accel";

raw_accel_entry::raw_accel_entry(double x, double y, double z) {
    add_data("x", x);
    add_data("y", y);
    add_data("z", z);
}

raw_accel_entry::~raw_accel_entry() {}

std::string raw_accel_entry::get_name() {
    return raw_accel_entry::ENTRY_ID;
}