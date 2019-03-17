//
// Created by Liam Kelly on 1/31/19.
//

#ifndef PROJECT_ACK_PACKET_H
#define PROJECT_ACK_PACKET_H

#include "pixhawk_packet.h"

class ack_packet : public pixhawk_packet {

public:

    static const char* PACKET_NAME;

    ack_packet();
    ~ack_packet();

    const char* get_packet_type();
};


#endif //PROJECT_ACK_PACKET_H
