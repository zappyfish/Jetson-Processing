//
// Created by Liam Kelly on 11/15/18.
//

#include "flight_packet.h"
#include <string>
#include <cstdio>
#include <cstring>

const size_t flight_packet::NUM_KEYS = 6;
const size_t flight_packet::PRECISION = 10;
const double flight_packet::ANGLE_SCALE_FACTOR = 0.01;

const char* flight_packet::PACKET_NAME = "flight_packet";
const char* flight_packet::KEYS[] = {"x", "y", "z", "roll", "pitch", "yaw"};

flight_packet::flight_packet(std::vector<const char*> keys, std::vector<const char*> values) {
    for (size_t i = 0; i < NUM_KEYS; i++) { // TODO: be careful with this
        m_keys.push_back(KEYS[i]);
        char * value = new char[PRECISION + 1];
        uint8_t len = std::strlen(values[i]);
        if (len > PRECISION) {
            len = PRECISION;
        }
        for (uint8_t j = 0; j < len; j++) {
            value[j] = values[i][j];
        }
        value[len] = '\0';
        m_values.push_back(value);
    }
}

flight_packet::flight_packet(int16_t x, int16_t y, int16_t z, int16_t roll, int16_t pitch, uint16_t yaw) {
    for (size_t i = 0; i < NUM_KEYS; i ++) {
        m_keys.push_back(KEYS[i]);
    }
    char *value = new char[PRECISION + 1];
    m_values.push_back(to_string(x, value));

    value = new char[PRECISION + 1];
    m_values.push_back(to_string(y, value));

    value = new char[PRECISION + 1];
    m_values.push_back(to_string(z, value));

    value = new char[PRECISION + 1];
    m_values.push_back(to_string(roll, value));

    value = new char[PRECISION + 1];
    m_values.push_back(to_string(pitch, value));

    value = new char[PRECISION + 1];
    m_values.push_back(to_string(yaw, value));

}

flight_packet::~flight_packet() {
    for (size_t i = 0; i < m_values.size(); i++) {
        delete[] m_values[i];
    }
}

double flight_packet::get_x() {
    return std::atof(m_values.at(0));
}

double flight_packet::get_y() {
    return std::atof(m_values.at(1));
}

double flight_packet::get_z() {
    return std::atof(m_values.at(2));
}

double flight_packet::get_roll() {
    return std::atoi(m_values.at(3)) * ANGLE_SCALE_FACTOR;
}

double flight_packet::get_pitch() {
    return std::atoi(m_values.at(4)) * ANGLE_SCALE_FACTOR;
}

double flight_packet::get_yaw() {
    return std::atoi(m_values.at(5)) * ANGLE_SCALE_FACTOR;
}

const char* flight_packet::get_packet_type() {
    return flight_packet::PACKET_NAME;
}


const char* flight_packet::to_string(int32_t value, char *buffer) {
    uint16_t positive_value;
    if (value < 0) {
        buffer[0] = '-';
        positive_value = (uint16_t)(value * -1);
        to_string(positive_value, &(buffer[1]), PRECISION - 1); // filled the first place with a negative, now move forward 1 place
    } else {
        positive_value = (uint16_t)value;
        to_string(positive_value, buffer, PRECISION);
    }
    return buffer;
}

void flight_packet::to_string(uint32_t value, char *buffer, int precision) {
    // Edge Case
    if (value == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }
    char tmp[precision + 1];
    int ind = 0;
    for (; ind < precision && value != 0; ind++) {
        tmp[ind] = ('0' + value % 10); // This is backwards, so we'll reverse it in the next loop
        value = value / 10;
    }
    for (int i = 0; i < ind; i++) {
        buffer[i] = tmp[ind - i - 1];
    }
    buffer[ind] = '\0'; // We filled in 'ind' digits, so the ind'th index should be a string terminator
}