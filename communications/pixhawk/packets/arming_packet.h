//
// Created by Liam Kelly on 1/31/19.
//

#ifndef PROJECT_ARMING_PACKET_H
#define PROJECT_ARMING_PACKET_H

#include "pixhawk_packet.h"

class arming_packet : public pixhawk_packet {

public:

    static const char* PACKET_NAME;

    static const char* SHOULD_ARM_ACTION;
    static const char* SHOULD_DISARM_ACTION;

    arming_packet(bool should_arm_action);
    arming_packet(std::vector<const char*> keys, std::vector<const char*> values);
    ~arming_packet();

    bool should_be_armed();

    const char* get_packet_type();

private:

    bool m_should_be_armed;

};


#endif //PROJECT_ARMING_PACKET_H
