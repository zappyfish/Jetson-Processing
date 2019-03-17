//
// Created by Liam Kelly on 2/14/19.
//

#include "directions_packet.h"

const char* directions_packet::PACKET_NAME = "directions";
const char* directions_packet::TARGET_DETECTED = "true";
const char* directions_packet::NO_TARGET = "false";
const char* directions_packet::TARGET_KEY = "saw_target";
const char* directions_packet::X_KEY = "x";
const char* directions_packet::Y_KEY = "y";
const char* directions_packet::LANDING_KEY = "should_land";
const char* directions_packet::SHOULD_LAND = "true";
const char* directions_packet::DO_NOT_LAND = "false";

const uint8_t directions_packet::PRECISION = 10;

directions_packet::directions_packet(int x, int y, bool saw_target, bool should_land) : m_x(x), m_y(y),
m_saw_target(saw_target), m_should_land(should_land) {
    m_keys.push_back(X_KEY);
    char *x_string = new char[PRECISION + 1];
    m_values.push_back(to_string(x, x_string));
    m_keys.push_back(Y_KEY);
    char *y_string = new char[PRECISION + 1];
    m_values.push_back(to_string(y, y_string));
    m_keys.push_back(TARGET_KEY);
    if (saw_target) {
        m_values.push_back(TARGET_DETECTED);
    } else {
        m_values.push_back(NO_TARGET);
    }
    m_keys.push_back(LANDING_KEY);
    if (should_land) {
        m_values.push_back(SHOULD_LAND);
    } else {
        m_values.push_back(DO_NOT_LAND);
    }
}

directions_packet::directions_packet(std::vector<const char *> keys, std::vector<const char *> values) {
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
            char * value = new char[std::strlen(values[i]) + 1];
            std::strcpy(value, values[i]);
            m_values.push_back(value);
        } else if (std::strcmp(keys[i], TARGET_KEY) == 0) {
            m_saw_target = (std::strlen(values[i]) > 0 && values[i][0] == 't');
            m_keys.push_back(TARGET_KEY);
            if (m_saw_target) {
                m_values.push_back(TARGET_DETECTED);
            } else {
                m_values.push_back(NO_TARGET);
            }
        } else if (std::strcmp(keys[i], LANDING_KEY) == 0) {
            m_should_land = (std::strlen(values[i]) > 0 && values[i][0] == 't');
            m_keys.push_back(LANDING_KEY);
            if (m_should_land) {
                m_values.push_back(SHOULD_LAND);
            } else {
                m_values.push_back(DO_NOT_LAND);
            }
        }

    }
}

directions_packet::~directions_packet() {
    for (size_t i = 0; i < m_values.size(); i++) {
        if (m_keys[i] == X_KEY || m_keys[i] == Y_KEY) {
            delete m_values[i]; // Otherwise, we store using the target/land values which are on the stack
        }
    }
}

int directions_packet::get_y() {
    return m_y;
}

int directions_packet::get_x() {
    return m_x;
}

bool directions_packet::get_saw_target() {
    return m_saw_target;
}

bool directions_packet::get_should_land() {
    return m_should_land;
}

const char* directions_packet::to_string(int value, char *buffer) {
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

const char* directions_packet::get_packet_type() {
    return directions_packet::PACKET_NAME;
}