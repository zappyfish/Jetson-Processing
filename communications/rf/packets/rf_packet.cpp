//
// Created by Liam Kelly on 10/27/18.
//

#include "rf_packet.h"

rf_packet::rf_packet(int gps_x, int gps_y) {
    m_x = gps_x;
    m_y = gps_y;
}

rf_packet::rf_packet(uint8_t *buf_data, uint8_t len) {
    m_x = (buf_data[0]) | (buf_data[1] << 8) | (buf_data[2] << 16) | (buf_data[3] << 24);
    m_y = (buf_data[4]) | (buf_data[5] << 8) | (buf_data[6] << 16) | (buf_data[7] << 24);
}

rf_packet::~rf_packet() {}

vector<uint8_t> rf_packet::serialize() {
    std::vector<uint8_t> serialized;
    for (int i = 0; i < 4; i++) {
        uint8_t b = ((m_x >> (8 * i)) & 0xff);
        serialized.push_back(b);
    }
    for (int i = 0; i < 4; i++) {
        uint8_t b = ((m_y >> (8 * i)) & 0xff);
        serialized.push_back(b);
    }
    return serialized;
}

int rf_packet::get_gps_x() {
    return m_x;
}

int rf_packet::get_gps_y() {
    return m_y;
}