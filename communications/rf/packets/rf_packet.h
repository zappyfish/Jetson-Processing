//
// Created by Liam Kelly on 10/27/18.
//

#ifndef NVIDIA_RF_PACKET_H
#define NVIDIA_RF_PACKET_H

#include <vector>
#include <cstdint>

using namespace std;

class rf_packet {

public:

    static const int OFFSET;

    /**
     * NOTE: these should be scaled -100 to 100
     */
    rf_packet(int gps_x, int gps_y);
    rf_packet(uint8_t *buf_data, uint8_t len);
    ~rf_packet();

    vector<uint8_t> serialize();

    int get_gps_x();
    int get_gps_y();

private:

    uint8_t m_x;
    uint8_t m_y;

    uint8_t m_buf_write_buf[2];
};


#endif //NVIDIA_RF_PACKET_H
