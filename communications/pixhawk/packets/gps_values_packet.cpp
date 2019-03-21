//
// Created by Liam Kelly on 3/21/19.
//

#include "gps_values_packet.h"


const char* gps_values_packet::PACKET_NAME = "gps_values";
const char* gps_values_packet::X_KEY = "x";
const char* gps_values_packet::Y_KEY = "y";

const uint8_t directions_packet::PRECISION = 10;

gps_values_packet::gps_values_packet(int x, int y) : m_x(x), m_y(y) {
    m_keys.push_back(X_KEY);
    char *x_string = new char[PRECISION + 1];
    m_values.push_back(to_string(x, x_string));
    m_keys.push_back(Y_KEY);
    char *y_string = new char[PRECISION + 1];
    m_values.push_back(to_string(y, y_string));
}

gps_values_packet::gps_values_packet(std::vector<const char *> keys, std::vector<const char *> values) {
    for (size_t i = 0; i < keys.size(); i++) {
        // check which key it is
        if (std::strcmp(keys[i], X_KEY) == 0) {
            m_x = atoi(values[i]);
            m_keys.push_back(X_KEY);
            char * value = new char[std::strlen(values[i]) + 1];
            std::strcpy(value, values[i]);
            m_values.push_back(value);
        } else if (std::strcmp(keys[i], Y_KEY) == 0) {
            m_y = atoi(values[i]);
            m_keys.push_back(Y_KEY);
            char *value = new char[std::strlen(values[i]) + 1];
            std::strcpy(value, values[i]);
            m_values.push_back(value);
        }
    }
}

gps_values_packet::~gps_values_packet() {
    for (size_t i = 0; i < m_values.size(); i++) {
        if (m_keys[i] == X_KEY || m_keys[i] == Y_KEY) {
            delete m_values[i]; // Otherwise, we store using the target/land values which are on the stack
        }
    }
}

int gps_values_packet::get_y() {
    return m_y;
}

int gps_values_packet::get_x() {
    return m_x;
}

const char* gps_values_packet::to_string(int value, char *buffer) {
    // Edge Case
    if (value == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return buffer;
    }
    char tmp[PRECISION + 1];
    for (size_t i = 0; i < PRECISION + 1; i++) {
        tmp[i] = ' ';
    }
    if (value < 0) {
        tmp[PRECISION] = '-';
        value *= -1;
    }
    for (size_t i = 0; i < PRECISION && value != 0; i++) {
        tmp[i] = ('0' + value % 10);
        value = value / 10;
    }
    for (int i = PRECISION; i >= 0; i--) {
        buffer[PRECISION - i] = tmp[i];
        if (i == 0 || tmp[i - 1] == ' ') {
            buffer[(PRECISION - i) + 1] = '\0';
        }
    }
    return buffer;
}

const char* gps_values_packet::get_packet_type() {
    return gps_values_packet::PACKET_NAME;
}