//
// Created by Liam Kelly on 10/27/18.
//

#include "rf_packet.h"

const int rf_packet::OFFSET = 100;

rf_packet::rf_packet(int gps_x, int gps_y) {
    m_x = (uint8_t)((gps_x + OFFSET) & 0xff);
    m_y = (uint8_t)((gps_y + OFFSET) & 0xff);
}

rf_packet::rf_packet(uint8_t *buf_data, uint8_t len) {
    m_x = buf_data[0];
    m_y = buf_data[1];
}

rf_packet::~rf_packet() {}

vector<uint8_t> rf_packet::serialize() {
    std::vector<uint8_t> serialized;
    serialized.push_back(m_x);
    serialized.push_back(m_y);
    return serialized;
}

int rf_packet::get_gps_x() {
    return (int)m_x - OFFSET;
}

int rf_packet::get_gps_y() {
    return (int)m_y - OFFSET;
}