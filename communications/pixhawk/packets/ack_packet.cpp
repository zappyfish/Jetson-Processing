//
// Created by Liam Kelly on 1/31/19.
//

#include "ack_packet.h"

const char* ack_packet::PACKET_NAME = "acknowledged";

ack_packet::ack_packet() {
}

ack_packet::~ack_packet() {}

const char* ack_packet::get_packet_type() {
    return ack_packet::PACKET_NAME;
}