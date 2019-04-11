//
// Created by Liam Kelly on 4/1/19.
//

#include "beacon_packet.h"

const char* beacon_packet::PACKET_NAME = "beacon_deploy";

beacon_packet::beacon_packet() {
}

beacon_packet::~beacon_packet() {}

const char* beacon_packet::get_packet_type() {
    return beacon_packet::PACKET_NAME;
}