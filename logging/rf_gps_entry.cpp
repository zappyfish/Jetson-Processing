//
// Created by Liam Kelly on 3/28/19.
//

#include "rf_gps_entry.h"

const std::string rf_gps_entry::ENTRY_ID = "rf_gps";

rf_gps_entry::rf_gps_entry(int x, int y) {
    add_data("x", x);
    add_data("y", y);
}

rf_gps_entry::~rf_gps_entry() {}

std::string rf_gps_entry::get_name() {
    return rf_gps_entry::ENTRY_ID;
}