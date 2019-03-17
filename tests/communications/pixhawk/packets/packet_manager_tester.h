//
// Created by Liam Kelly on 11/13/18.
//

#ifndef NVIDIA_PACKET_MANAGER_TESTER_H
#define NVIDIA_PACKET_MANAGER_TESTER_H

#include "packet_manager.h"
#include "test_packet.h"
#include "directions_packet.h"
#include "test_uart.h"
#include "flight_packet.h"
#include "pixhawk_entry.h"

void packet_manager_test_callback(const char *name, std::vector<const char *> keys, std::vector <const char *> values, void *args);
void packet_manager_empty_callback(const char *name, std::vector<const char *> keys, std::vector <const char *> values, void *args);

/**
 * In this test case, we ensure that the packet manager correctly returns the number of packets handled.
 */
TEST_CASE("Packet Manager Test Gets Number of Packets Available Correctly", "[packet_manager]") {
    test_uart *uart = new test_uart();

    packet_manager::get_instance().set_uart(uart);

    packet_manager::packet_callback p_callback;
    p_callback.callback = &packet_manager_empty_callback;
    p_callback.name = test_packet::PACKET_NAME;

    packet_manager::get_instance().set_packet_callback(&p_callback);

    REQUIRE(packet_manager::get_instance().check_packets() == 0); // Nothing sent so far

    std::vector<char> data;
    std::string invalid_string;
    invalid_string.append(test_packet::PACKET_NAME);
    invalid_string.append(":key1,val1,key2,val2,not_terminated_correctly");
    data = std::vector<char>(invalid_string.begin(), invalid_string.end());
    uart->write_buffer(data);

    invalid_string.clear();

    // We didn't get the right format, so shouldn't get anything
    REQUIRE(packet_manager::get_instance().check_packets() == 0);

    invalid_string.append(test_packet::PACKET_NAME);
    invalid_string.append(":correct,packet,but,incorrect,checksum,lelz;*");

    data = std::vector<char>(invalid_string.begin(), invalid_string.end());

    uart->write_buffer(data);

    REQUIRE(packet_manager::get_instance().check_packets() == 0);

    std::vector<const char*> keys;
    std::vector<const char*> values;

    keys.push_back("lmao");
    values.push_back("8");

    keys.push_back("memes");
    values.push_back("imtired");

    test_packet *packet = new test_packet(keys, values);

    std::vector<char> s = packet->serialize();

    packet_manager::get_instance().send_packet(packet); // This works EXCLUSIVELY for test_uart

    REQUIRE(packet_manager::get_instance().check_packets() == 1); // Correct packet type

    // Now that the packet is handled, make sure size is updated (i.e. shouldn't be any packets handle this time)
    REQUIRE(packet_manager::get_instance().check_packets() == 0);
}


TEST_CASE("Packet Manager Test Data Sent and Received Correctly", "[packet_manager]") {
    test_uart *uart = new test_uart();

    packet_manager::get_instance().set_uart(uart);

    std::vector<const char*> keys;
    std::vector<const char*> values;

    keys.push_back("lmao");
    values.push_back("8");

    keys.push_back("memes");
    values.push_back("imtired");

    std::vector<char> r_v;

    // Our callback:
    packet_manager::packet_callback p_callback;
    p_callback.callback = &packet_manager_test_callback;
    p_callback.name = test_packet::PACKET_NAME;
    p_callback.args = (void *)&r_v;

    packet_manager::get_instance().set_packet_callback(&p_callback);

    test_packet *sent = new test_packet(keys, values);
    std::vector<char> s_v = sent->serialize();

    packet_manager::get_instance().send_packet(sent);

    packet_manager::get_instance().check_packets();
    // Now, given our callback, we know that r_v should be set to the last sent packet

    REQUIRE(s_v.size() == r_v.size());

    for (size_t i = 0; i < s_v.size(); i++) {
        REQUIRE(s_v.at(i) == r_v.at(i));
    }
}

TEST_CASE("Packet Manager Test Can Receive Packet In Broken Up Pieces", "[packet_manager]") {
    test_uart *uart = new test_uart();

    packet_manager::get_instance().set_uart(uart);

    flight_packet *sent = new flight_packet(1, 2, 3, 4, 5, 6);

    std::vector<char> serialized = sent->serialize();

    for (size_t i = 0; i < serialized.size(); i++) {
        REQUIRE(packet_manager::get_instance().check_packets() == 0);
        uart->write_single_char(serialized.at(i));
    }

    // Should only have received it once every character is sent
    REQUIRE(packet_manager::get_instance().check_packets() == 1);

    delete sent;

}

TEST_CASE("Packet Manager Test Remove Callback", "[packet_manager]") {
test_uart *uart = new test_uart();

    packet_manager::get_instance().set_uart(uart);

    std::vector<const char*> keys;
    std::vector<const char*> values;

    keys.push_back("lmao");
    values.push_back("8");

    keys.push_back("memes");
    values.push_back("imtired");

    std::vector<char> r_v;

    // Our callback:
    packet_manager::packet_callback p_callback;
    p_callback.callback = &packet_manager_test_callback;
    p_callback.name = test_packet::PACKET_NAME;
    p_callback.args = (void *)&r_v;

    packet_manager::get_instance().set_packet_callback(&p_callback);

    test_packet *sent = new test_packet(keys, values);
    std::vector<char> s_v = sent->serialize();

    packet_manager::get_instance().send_packet(sent);

    packet_manager::get_instance().check_packets();
    // Now, given our callback, we know that r_v should be set to the last sent packet

    REQUIRE(s_v.size() == r_v.size());

    for (size_t i = 0; i < s_v.size(); i++) {
        REQUIRE(s_v.at(i) == r_v.at(i));
    }

    r_v.clear();

    packet_manager::get_instance().remove_packet_callback(&p_callback);

    sent = new test_packet(keys, values);

    packet_manager::get_instance().send_packet(sent);

    packet_manager::get_instance().check_packets();

    REQUIRE(s_v.size() != r_v.size());

    REQUIRE(r_v.size() == 0);
}

void packet_manager_test_callback(const char *name, std::vector<const char *> keys, std::vector <const char *> values, void *args) {
    std::vector<char> *r_v = static_cast<std::vector<char> *> (args);
    r_v->clear();
    test_packet packet(keys, values);
    std::vector<char> serialized = packet.serialize();
    for (size_t i = 0; i < serialized.size(); i++) {
        r_v->push_back(serialized.at(i));
    }
}

void packet_manager_empty_callback(const char *name, std::vector<const char *> keys, std::vector <const char *> values, void *args) {

}

#endif //NVIDIA_PACKET_MANAGER_TESTER_H