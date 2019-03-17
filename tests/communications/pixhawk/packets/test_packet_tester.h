//
// Created by Liam Kelly on 11/13/18.
//

#ifndef NVIDIA_PIXHAWK_PACKET_TESTER_H
#define NVIDIA_PIXHAWK_PACKET_TESTER_H

#include "test_packet.h"

/**
 * In this test case, we use the test_packet class, inherited from pixhawk_packet, to make sure serialize works as
 * expected.
 */
TEST_CASE("Test Pixhawk Packet Serializes As Expected", "[test_packet]") {
    std::vector<const char*> keys;
    std::vector<const char*> values;

    keys.push_back("lmao");
    values.push_back("8");

    keys.push_back("memes");
    values.push_back("imtired");
    test_packet packet(keys, values);
    char checksum = 46; // 46 is result of XOR checksum on "lmao8memesimtired"
    std::string expected = "test:lmao,8,memes,imtired^";
    expected.push_back(checksum);
    expected.push_back(';');

    std::vector<char> serialized = packet.serialize();
    std::string received = std::string(serialized.begin(), serialized.end());

    REQUIRE(received == expected);
}

#endif //NVIDIA_PIXHAWK_PACKET_TESTER_H
