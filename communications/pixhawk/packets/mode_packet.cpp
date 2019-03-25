//
// Created by Liam Kelly on 2/20/19.
//

#include "mode_packet.h"

const char* mode_packet::PACKET_NAME = "mode";
const char* mode_packet::AUTONOMOUS_KEY = "autonomous";
const char* mode_packet::IN_TARGET = "target";
const char* mode_packet::IN_MANUAL = "false";
const char* mode_packet::IN_GPS = "gps";

mode_packet::mode_packet(bool is_autonomous, bool is_target) : m_is_autonomous(is_autonomous), m_is_target(is_target) {
    m_keys.push_back(AUTONOMOUS_KEY);
    if (is_autonomous) {
        if (is_target) {
            m_values.push_back(IN_TARGET);
        } else {
            m_values.push_back(IN_GPS);
        }
    } else {
        m_values.push_back(IN_MANUAL);
    }
}

mode_packet::mode_packet(std::vector<const char *> keys, std::vector<const char *> values) {
    if (keys.size() > 0 && values.size() > 0) {
        m_keys.push_back(AUTONOMOUS_KEY);
        if (std::strlen(values.at(0)) > 0 && values.at(0)[0] == 't') {
            m_values.push_back(IN_AUTONOMOUS);
            m_is_autonomous = true;
            m_is_target = true;
        } else if (std::strlen(values.at(0) && values.at(0)[0] == 'g')) {
            m_values.push_back(IN_GPS);
            m_is_autonomous = true;
            m_is_target = false;
        } else {
            m_values.push_back(IN_MANUAL);
            m_is_autonomous = false;
        }
    }
}

mode_packet::~mode_packet() {}

bool mode_packet::get_is_autonomous() {
    return m_is_autonomous;
}

bool mode_packet::get_is_target() {
    return m_is_target;
}

const char* mode_packet::get_packet_type() {
    return mode_packet::PACKET_NAME;
}