//
// Created by Liam Kelly on 2/20/19.
//

#ifndef PROJECT_MODE_PACKET_H
#define PROJECT_MODE_PACKET_H

#include "pixhawk_packet.h"
#include <cstring>
#include <vector>

class mode_packet : public pixhawk_packet {

public:

    static const char* PACKET_NAME;
    static const char* AUTONOMOUS_KEY;
    static const char* IN_AUTONOMOUS;
    static const char* IN_MANUAL;

    mode_packet(bool is_autonomous);
    mode_packet(std::vector<const char*> keys, std::vector<const char*> values);;
    ~mode_packet();

    bool get_is_autonomous();

    const char* get_packet_type();

private:

    bool m_is_autonomous;

};


#endif //PROJECT_MODE_PACKET_H
