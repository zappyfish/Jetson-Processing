//
// Created by Liam Kelly on 11/12/18.
//

#include "pose_packet.h"

const char* pose_packet::PACKET_NAME = "pose";

const char* pose_packet::get_packet_type() {
    return pose_packet::PACKET_NAME;
}

pose_packet::pose_packet(vector<const char*> keys, vector<const char*> values) {}
pose_packet::~pose_packet() {}

bool pose_packet::is_heading_new() {
    return false;
}
bool pose_packet::is_pose_new() {
    return false;
}
