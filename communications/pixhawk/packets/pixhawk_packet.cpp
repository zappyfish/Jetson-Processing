//
// Created by Liam Kelly on 10/27/18.
//

#include "pixhawk_packet.h"
#include <string.h>
#include <cstring>

std::vector<char> pixhawk_packet::serialize() {
    std::vector<char> serialized;
    char checksum = 0;
    // NOTE: we do not use the packet type in the checksum
    for (size_t i = 0; i < std::strlen(get_packet_type()); i++) {
        char next = get_packet_type()[i];
        serialized.push_back(next);
    }

    serialized.push_back(PACKET_START);
    size_t fields_cnt = 0;
    while (fields_cnt < m_keys.size()) {
        if (fields_cnt > 0) {
            serialized.push_back(DELIMITER);
        }
        const char *key = m_keys[fields_cnt];
        for (size_t i = 0; i < std::strlen(key); i++) {
            char next = key[i];
            if (next != ' ') {
                serialized.push_back(next);
                checksum = checksum ^ next;
            }
        }
        serialized.push_back(DELIMITER);
        const char *val = m_values[fields_cnt++];
        for(size_t i = 0; i < std::strlen(val); i++) {
            char next = val[i];
            if (next != ' ') {
                serialized.push_back(next);
                checksum = checksum ^ next;
            }
        }
    }
    serialized.push_back(END_OF_TOKENS);
    serialized.push_back(checksum);
    serialized.push_back(PACKET_END);
    return serialized;
}

const char* pixhawk_packet::get_packet_type() {
    return "base";
}