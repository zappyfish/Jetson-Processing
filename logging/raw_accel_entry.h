//
// Created by Liam Kelly on 4/5/19.
//

#ifndef PROJECT_RAW_ACCEL_ENTRY_H
#define PROJECT_RAW_ACCEL_ENTRY_H

#include "log_entry.h"

class raw_accel_entry : public log_entry {

public:

        raw_accel_entry(double x, double y, double z);
        ~raw_accel_entry();

        std::string get_name();

private:

        static const std::string ENTRY_ID;

};


#endif //PROJECT_RAW_ACCEL_ENTRY_H
