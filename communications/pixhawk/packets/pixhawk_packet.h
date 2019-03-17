//
// Created by Liam Kelly on 10/27/18.
//

#ifndef NVIDIA_PIXHAWK_PACKET_H
#define NVIDIA_PIXHAWK_PACKET_H

#include <vector>
#include <memory>

/**
 * On both sides, we have a pixhawk packet. We serialize it for transmission, and then
 * deserialize via the constructor on the receiver side.
 */

class pixhawk_packet {

public:

    enum CONSTANTS {
        PACKET_END = ';',
        PACKET_START = ':',
        DELIMITER = ',',
        END_OF_TOKENS = '^'
    };

    virtual ~pixhawk_packet() {}

    std::vector<char> serialize();

    static const char* PACKET_NAME;

    virtual const char* get_packet_type();

protected:

    std::vector<const char*> m_keys;
    std::vector<const char*> m_values;

};

namespace pixhawk {

    typedef std::shared_ptr<pixhawk_packet> packet_ptr;

}


#endif //NVIDIA_PIXHAWK_PACKET_H
