//
// Created by Liam Kelly on 2/7/19.
//

#include "target_entry.h"

const std::string target_entry::ENTRY_ID = "target";
const std::string target_entry::SHOULD_LAND = "should_land";
const std::string target_entry::LAND_TRUE = "true";
const std::string target_entry::LAND_FALSE = "false";

target_entry::target_entry(std::string image_timestamp, int target_x, int target_y, double distance_x, double distance_y,
                           double send_angle_x, double send_angle_y, bool should_land) {
    add_data(std::string("image_timestamp"), image_timestamp);
    add_data(std::string("x_coord"), target_x);
    add_data(std::string("y_coord"), target_y);
    add_data(std::string("send_x"), send_angle_x);
    add_data(std::string("send_y"), send_angle_y);
    add_data(std::string("distance_x"), distance_x);
    add_data(std::string("distance_y"), distance_y);
    if (should_land) {
        add_data(SHOULD_LAND, LAND_TRUE);
    } else {
        add_data(SHOULD_LAND, LAND_FALSE);
    }
}

target_entry::~target_entry() {}

std::string target_entry::get_name() { return target_entry::ENTRY_ID; }