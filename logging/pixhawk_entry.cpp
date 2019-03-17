//a
// Created by Liam Kelly on 11/15/18.
//

#include "pixhawk_entry.h"

const std::string pixhawk_entry::ENTRY_ID = "PIXHAWK_DATA";

pixhawk_entry::pixhawk_entry(flight_packet &packet, Vector3 &accel) {
    add_data("x", packet.get_x());
    add_data("y", packet.get_y());
    add_data("z", packet.get_z());
    add_data("roll", packet.get_roll());
    add_data("pitch", packet.get_pitch());
    add_data("yaw", packet.get_yaw());
    add_data("accel_x", accel.x);
    add_data("accel_y", accel.y);
    add_data("accel_z", accel.z);
}

std::string pixhawk_entry::get_name() {
    return pixhawk_entry::ENTRY_ID;
}