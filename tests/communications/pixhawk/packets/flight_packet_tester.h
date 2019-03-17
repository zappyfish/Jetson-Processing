//
// Created by Liam Kelly on 1/6/19.
//

#ifndef NVIDIA_FLIGHT_PACKET_TESTER_H
#define NVIDIA_FLIGHT_PACKET_TESTER_H

#endif //NVIDIA_FLIGHT_PACKET_TESTER_H

#include "flight_packet.h"

TEST_CASE("Test Serializing", "[flight_packet]") {
    flight_packet packet(10, 20, 30, 40, 50, 60);
    std::vector<char> serialized = packet.serialize();
    std::string str = std::string(serialized.begin(), serialized.end());

    REQUIRE(str == "flight_packet:x,10,y,20,z,30,roll,40,pitch,50,yaw,60^h;");
}

TEST_CASE("Test Getters", "[flight_packet]") {
    flight_packet packet(10, -200, 3000, 1, -2389, 39819);

    REQUIRE(packet.get_x() == 10);
    REQUIRE(packet.get_y() == -200);
    REQUIRE(packet.get_z() == 3000);
    REQUIRE(packet.get_roll() == 0.01);
    REQUIRE(packet.get_pitch() == -23.89);
    REQUIRE(packet.get_yaw() == 398.19); // only positive values
}

TEST_CASE("Test Key, Value Constructor", "[flight_packet]") {
    std::vector<const char*> keys;
    std::vector<const char*> vals;

    char x[] = "-22";
    char y[] = "10";
    char z[] = "0";

    char roll[] = "3000";
    char pitch[] = "4000";
    char yaw[] = "5000";

    char x_key[] = "x";
    char y_key[] = "y";
    char z_key[] = "z";

    char roll_key[] = "roll";
    char pitch_key[] = "pitch";
    char yaw_key[] = "yaw";


    keys.push_back(x_key);
    keys.push_back(y_key);
    keys.push_back(z_key);

    keys.push_back(roll_key);
    keys.push_back(pitch_key);
    keys.push_back(yaw_key);


    vals.push_back(x);
    vals.push_back(y);
    vals.push_back(z);

    vals.push_back(roll);
    vals.push_back(pitch);
    vals.push_back(yaw);

    flight_packet packet(keys, vals);

    REQUIRE(packet.get_x() == -22);
    REQUIRE(packet.get_y() == 10);
    REQUIRE(packet.get_z() == 0);
    REQUIRE(packet.get_roll() == 30);
    REQUIRE(packet.get_pitch() == 40);
    REQUIRE(packet.get_yaw() == 50);
}