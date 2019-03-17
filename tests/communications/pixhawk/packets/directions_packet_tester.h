//
// Created by Liam Kelly on 2/19/19.
//

#ifndef PROJECT_DIRECTIONS_PACKET_TESTER_H
#define PROJECT_DIRECTIONS_PACKET_TESTER_H

#include "directions_packet.h"
#include <vector>
#include <string>

TEST_CASE("Directions Packet Sends Negatives", "[directions_packet]") {
    directions_packet packet(-5, 5, true, false);

    std::string expected("directions:x,-5,y,5,saw_target,true,should_land,false^=;");
    std::vector<char> sent = packet.serialize();
    std::string received(sent.begin(), sent.end());
    REQUIRE(received == expected);

    // std::cout << packet.serialize() << std::endl;
}

#endif //PROJECT_DIRECTIONS_PACKET_TESTER_H
