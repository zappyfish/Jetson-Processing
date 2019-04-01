//
// Created by Liam Kelly on 4/1/19.
//

#ifndef PROJECT_BEACON_PACKET_H
#define PROJECT_BEACON_PACKET_H

#include "pixhawk_packet.h"

class beacon_packet : public pixhawk_packet {

public:

    static const char* PACKET_NAME;

    beacon_packet();
    ~beacon_packet();

    const char* get_packet_type();
};


#endif //PROJECT_BEACON_PACKET_H
