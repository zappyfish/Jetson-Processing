//
// Created by Liam Kelly on 2/28/19.
//

#ifndef PROJECT_ACK_PACKET_TESTER_H
#define PROJECT_ACK_PACKET_TESTER_H

#include "ack_packet.h"

TEST_CASE("Test Ack Packet", "[ack_packet]") {
    ack_packet p;

    std::vector<char> ser = p.serialize();
    std::string serialized(ser.begin(), ser.end());

    REQUIRE(serialized == "acknowledged:^;");
}

#endif //PROJECT_ACK_PACKET_TESTER_H
