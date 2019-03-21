//
// Created by Liam Kelly on 3/21/19.
//

#ifndef PROJECT_GPS_VALUES_PACKET_H
#define PROJECT_GPS_VALUES_PACKET_H

#include "pixhawk_packet.h"
#include <cstdlib>
#include <cstring>

class gps_values_packet : public pixhawk_packet {

public:

    static const char* PACKET_NAME;
    static const char* X_KEY;
    static const char* Y_KEY;

    static const uint8_t PRECISION;

    gps_values_packet(int x, int y);
    gps_values_packet(std::vector<const char*> keys, std::vector<const char*> values);
    ~gps_values_packet();

    int get_x();
    int get_y();

    const char* get_packet_type();

private:

    int m_x;
    int m_y;

    const char* to_string(int value, char *buffer);

};


#endif //PROJECT_GPS_VALUES_PACKET_H
