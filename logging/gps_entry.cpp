//
// Created by Liam Kelly on 3/22/19.
//

#include "gps_entry.h"

const std::string gps_entry::ENTRY_ID = "beacon_gps";

gps_entry::gps_entry(int x, int y) {
    add_data("x", x);
    add_data("y", y);
}

gps_entry::~gps_entry() {}

std::string gps_entry::get_name() {
    return gps_entry::ENTRY_ID;
}