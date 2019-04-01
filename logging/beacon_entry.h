//
// Created by Liam Kelly on 4/1/19.
//

#ifndef PROJECT_BEACON_ENTRY_H
#define PROJECT_BEACON_ENTRY_H

#include "log_entry.h"

class beacon_entry : public log_entry {

public:

    beacon_entry();
    ~beacon_entry();

    std::string get_name();

private:

    static const std::string ENTRY_ID;

};


#endif //PROJECT_BEACON_ENTRY_H
