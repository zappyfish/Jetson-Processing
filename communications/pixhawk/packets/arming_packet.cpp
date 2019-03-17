//
// Created by Liam Kelly on 1/31/19.
//

#include "arming_packet.h"
#include <cstring>

const char* arming_packet::SHOULD_ARM_ACTION = "arm";
const char* arming_packet::SHOULD_DISARM_ACTION = "disarm";
const char* arming_packet::PACKET_NAME = "arming";

arming_packet::arming_packet(bool should_arm_action) : m_should_be_armed(should_arm_action) {
    m_keys.push_back("action");
    if (m_should_be_armed) {
        m_values.push_back(SHOULD_ARM_ACTION);
    } else {
        m_values.push_back(SHOULD_DISARM_ACTION);
    }
}

arming_packet::arming_packet(std::vector<const char *> keys, std::vector<const char *> values) {
    if (keys.size() == 1 && values.size() == 1) {
        m_should_be_armed = (std::strcmp(SHOULD_ARM_ACTION, values.at(0)) == 0);
    } else {
        m_should_be_armed = true; // I guess? dunno
    }
}

arming_packet::~arming_packet() {}

bool arming_packet::should_be_armed() {
    return m_should_be_armed;
}

const char* arming_packet::get_packet_type() {
    return arming_packet::PACKET_NAME;
}

