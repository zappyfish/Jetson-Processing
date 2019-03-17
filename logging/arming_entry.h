//
// Created by Liam Kelly on 2/7/19.
//

#ifndef PROJECT_ARMING_ENTRY_H
#define PROJECT_ARMING_ENTRY_H

#include "log_entry.h"
#include "arming_packet.h"

class arming_entry : public log_entry {

public:

    arming_entry(arming_packet packet);
    ~arming_entry();

    std::string get_name();

private:

    static const std::string ENTRY_ID;

};


#endif //PROJECT_ARMING_ENTRY_H
