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
    static const char* IN_TARGET;
    static const char* IN_MANUAL;
    static const char* IN_GPS;

    mode_packet(bool is_autonomous, bool is_target);
    mode_packet(std::vector<const char*> keys, std::vector<const char*> values);;
    ~mode_packet();

    bool get_is_autonomous();
    bool get_is_target();

    const char* get_packet_type();

private:

    bool m_is_autonomous;
    bool m_is_target;

};


#endif //PROJECT_MODE_PACKET_H
