//
// Created by Liam Kelly on 2/7/19.
//

#ifndef PROJECT_TARGET_ENTRY_H
#define PROJECT_TARGET_ENTRY_H

#include "log_entry.h"

class target_entry : public log_entry {

public:

    target_entry(std::string image_timestamp, int target_x, int target_y, double distance_x, double distance_y,
            double send_angle_x, double send_angle_y, bool should_land);
    ~target_entry();

    std::string get_name();

private:

    static const std::string ENTRY_ID;
    static const std::string SHOULD_LAND;
    static const std::string LAND_TRUE;
    static const std::string LAND_FALSE;
};


#endif //PROJECT_TARGET_ENTRY_H
