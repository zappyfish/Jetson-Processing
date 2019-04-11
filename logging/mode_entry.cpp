//
// Created by Liam Kelly on 2/20/19.
//

#include "mode_entry.h"

const std::string mode_entry::ENTRY_ID = "mode";
const std::string mode_entry::AUTONOMOUS = "autonomous";
const std::string mode_entry::MANUAL = "manual";
const std::string mode_entry::GPS = "gps";

mode_entry::mode_entry(bool is_autonomous, bool is_target) {
    if (is_autonomous) {
        if (is_target) {
            add_data(ENTRY_ID, AUTONOMOUS);
        } else {
            add_data(ENTRY_ID, GPS);
        }
    } else {
        add_data(ENTRY_ID, MANUAL);
    }
}

mode_entry::~mode_entry() {}

std::string mode_entry::get_name() {
    return mode_entry::ENTRY_ID;
}