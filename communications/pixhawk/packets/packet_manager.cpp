//
// Created by Liam Kelly on 11/12/18.
//

#include "packet_manager.h"
#include "flight_packet.h"
#include "pose_packet.h"
#include <cstring>
#include <string.h>
#include "test_packet.h"
#include <iostream>

#if defined(__linux__)|| defined(_WIN32) || defined(__APPLE__)
#include "data_logger.h"
#endif

packet_manager& packet_manager::get_instance() {
    static packet_manager instance;
    return instance;
}

packet_manager::packet_manager() : m_uart(nullptr) {
    // no much to do here
}

packet_manager::~packet_manager() {
    if (m_uart != nullptr) {
        delete m_uart;
    }
}

void packet_manager::send_packet(pixhawk_packet *packet) {
    std::vector<char> serialized = packet->serialize();
    m_uart->write_buffer(serialized);
    delete packet;
}

size_t packet_manager::check_packets() {
    if (m_uart == nullptr) {
        return 0;
    }

    std::vector<char> buffer_data;
    m_uart->read_buffer(buffer_data);

    size_t packets_to_parse = 0;
    for (size_t i = 0; i < buffer_data.size(); i++) {
        char c = buffer_data.at(i);
        if (c == pixhawk_packet::PACKET_END) {
            packets_to_parse++;
        } else {
            m_buffer_data.push(buffer_data.at(i));
        }
    }

    size_t success_count = 0;

    while (packets_to_parse > 0) {
        if (parse_for_packet_data()) {
            success_count++;
        }
        packets_to_parse--;
    }

    return success_count;
}

bool packet_manager::parse_for_packet_data() {
    char checksum = 0;
    std::vector<char> next_token;
    std::vector<char> packet_name;
    std::vector<const char*> keys;
    std::vector<const char*> values;
    bool got_name = false;
    bool is_key = true;
    char c;
    while (!m_buffer_data.empty()) {
        c = m_buffer_data.front();
        m_buffer_data.pop();
        if (!got_name) {
            if (c != pixhawk_packet::PACKET_START) {
                packet_name.push_back(c);
            } else {
                got_name = true;
            }
        } else {
            switch (c) {
                case pixhawk_packet::END_OF_TOKENS: {
                    char *token = get_token_copy(next_token);
                    values.push_back(token); // Guaranteed to be a value if it's the last token
                    // Validate checksum here
                    if (m_buffer_data.empty() || m_buffer_data.front() != checksum) {
#if defined(__linux__)|| defined(_WIN32) || defined(__APPLE__)
//                        std::string category = "packet_manager";
//                        std::string description = "checksum did not match";
//                        std::map<std::string, std::string> metadata;
//                        metadata["file"] = "packet_manager.cpp";
//                        metadata["method"] = "parse_for_packet_data";
//                        soft_error_entry *s_error = new soft_error_entry(category, description, metadata);
//                        data_logger::get_instance().save_log_entry(s_error);
#endif
                    } else {
                        char *name = get_token_copy(packet_name);
                        handle_packet(name, keys, values);
                        delete[] name;
                    }
                    for (size_t k = 0; k < keys.size(); k++) {
                        delete[] keys.data()[k];
                    }
                    for (size_t v = 0; v < values.size(); v++) {
                        delete[] values.data()[v];
                    }
                    if (!m_buffer_data.empty()) { // Move past the checksum if it was there
                        m_buffer_data.pop();
                    }
                    return true;
                }

                case pixhawk_packet::DELIMITER: {
                    char *token = get_token_copy(next_token);
                    if (is_key) {
                        // We're recording a value and reached the end, so store the key-value pair and reset
                        keys.push_back(token);
                    } else {
                        values.push_back(token);
                    }
                    is_key = !is_key;
                    break;
                }

                default: {
                    next_token.push_back(c);
                    checksum = checksum ^ c;
                    break;
                }
            }
        }
    }
    for (size_t k = 0; k < keys.size(); k++) {
        delete[] keys.data()[k];
    }
    for (size_t v = 0; v < values.size(); v++) {
        delete[] values.data()[v];
    }
    return false;
}

/**
 * Call all packet callbacks associated with a packet type
 * @param packet_type the name of the packet
 * @param keys the keys collected
 * @param values the values collected
 */
void packet_manager::handle_packet(const char* packet_type, std::vector<const char*> keys, std::vector<const char*> values) {
    std::cout << packet_type << std::endl;
    for (size_t i = 0; i < m_packet_callbacks.size(); i++) {
        packet_callback *p_callback = m_packet_callbacks.data()[i];
        if (std::strcmp(p_callback->name, packet_type) == 0) { // This callback is assigned to this packet_type, so invoke its callback
            p_callback->callback(packet_type, keys, values, p_callback->args);
        }
    }
}

void packet_manager::set_uart(uart *uart_bus) {
    m_uart = uart_bus;
}

void packet_manager::set_packet_callback(packet_callback *callback) {
    // See if there is already a callback for this packet
    for (size_t i = 0; i < m_packet_callbacks.size(); i++) {
        if (m_packet_callbacks.data()[i] == callback) {
            return; // Same reference, so need to do anything
        }
    }
    m_packet_callbacks.push_back(callback); // New reference, so replace it
}

bool packet_manager::remove_packet_callback(packet_manager::packet_callback *callback) {
    for (size_t search = 0; search < m_packet_callbacks.size(); search++) {
        if (m_packet_callbacks.data()[search] == callback) {
            for (size_t i = search + 1; i < m_packet_callbacks.size(); i++) {
                m_packet_callbacks.data()[i - 1] = m_packet_callbacks.data()[i]; // overwrite the one we're removing
            }
            m_packet_callbacks.pop_back(); // remove the last duplicate
            return true;
        }
    }
    return false;
}

char* packet_manager::get_token_copy(std::vector<char> &next_token) {
    char *token = new char[next_token.size() + 1];
    for (size_t i = 0; i < next_token.size(); i++) {
        token[i] = next_token.data()[i];
    }
    token[next_token.size()] = '\0';
    next_token.clear();
    return token;
}
