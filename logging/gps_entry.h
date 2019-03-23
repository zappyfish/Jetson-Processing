//
// Created by Liam Kelly on 3/22/19.
//

#ifndef PROJECT_GPS_ENTRY_H
#define PROJECT_GPS_ENTRY_H

#include "log_entry.h"

class gps_entry: public log_entry{

public:

    gps_entry(int x, int y);
    ~gps_entry();

    std::string get_name();

private:

    static const std::string ENTRY_ID;
};


#endif //PROJECT_GPS_ENTRY_H
