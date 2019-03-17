//
// Created by Liam Kelly on 2/7/19.
//

#include "arming_entry.h"

const std::string arming_entry::ENTRY_ID = "ARMING";

arming_entry::arming_entry(arming_packet packet) {
    if (packet.should_be_armed()) {
        add_data("arming_action", "arm");
    } else {
        add_data("arming_action", "disarm");
    }
}

arming_entry::~arming_entry() {}

std::string arming_entry::get_name() {
    return arming_entry::ENTRY_ID;
}