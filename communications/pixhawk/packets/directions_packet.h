//
// Created by Liam Kelly on 2/14/19.
//

#ifndef PROJECT_DIRECTIONS_PACKET_H
#define PROJECT_DIRECTIONS_PACKET_H

#include "pixhawk_packet.h"
#include <cstdlib>
#include <cstring>

class directions_packet : public pixhawk_packet {

public:

    static const char* PACKET_NAME;
    static const char* TARGET_DETECTED;
    static const char* NO_TARGET;
    static const char* TARGET_KEY;
    static const char* X_KEY;
    static const char* Y_KEY;
    static const char* LANDING_KEY;
    static const char* SHOULD_LAND;
    static const char* DO_NOT_LAND;

    static const uint8_t PRECISION;

    directions_packet(int x, int y, bool saw_target, bool should_land);
    directions_packet(std::vector<const char*> keys, std::vector<const char*> values);
    ~directions_packet();

    int get_x();
    int get_y();
    bool get_saw_target();
    bool get_should_land();

    const char* get_packet_type();

private:

    int m_x;
    int m_y;

    bool m_saw_target;
    bool m_should_land;

    const char* to_string(int value, char *buffer);

};


#endif //PROJECT_DIRECTIONS_PACKET_H
