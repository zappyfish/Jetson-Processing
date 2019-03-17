//
// Created by Liam Kelly on 11/15/18.
//

#ifndef ARDUCOPTER_PIXHAWK_ENTRY_H
#define ARDUCOPTER_PIXHAWK_ENTRY_H

#include "flight_packet.h"
#include "log_entry.h"
#include "vector_3.h"

class pixhawk_entry : public log_entry {

public:
    pixhawk_entry(flight_packet &packet, Vector3 &accel);

    std::string get_name();

private:

    static const std::string ENTRY_ID;
};


#endif //ARDUCOPTER_PIXHAWK_ENTRY_H