//
// Created by Liam Kelly on 11/13/18.
//

#include "test_packet.h"
#include <cstring>

const char * test_packet::PACKET_NAME = "test";

test_packet::test_packet(std::vector<const char*> keys, std::vector<const char*> values) {
    for (size_t i = 0; i < keys.size(); i++) {
        char *key = new char[std::strlen(keys[i]) + 1];
        std::strcpy(key, keys[i]);
        char *val = new char[std::strlen(values[i]) + 1];
        std::strcpy(val, values[i]);
        m_keys.push_back(key);
        m_values.push_back(val);
    }
}

test_packet::~test_packet() {
    for (size_t i = 0; i < m_keys.size(); i++) {
        delete[] m_keys[i];
        delete[] m_values[i];
    }
}


const char* test_packet::get_packet_type() {
    return test_packet::PACKET_NAME;
}