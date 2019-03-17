//
// Created by Liam Kelly on 11/13/18.
//

#ifndef NVIDIA_TEST_PACKET_H
#define NVIDIA_TEST_PACKET_H

#include "pixhawk_packet.h"

class test_packet : public pixhawk_packet {

public:

    static const char* PACKET_NAME;

    test_packet(std::vector<const char*> keys, std::vector<const char*> values);
    ~test_packet();

    const char* get_packet_type();

};


#endif //NVIDIA_TEST_PACKET_H
