//
// Created by Liam Kelly on 2/20/19.
//

#ifndef PROJECT_MODE_ENTRY_H
#define PROJECT_MODE_ENTRY_H

#include "log_entry.h"

class mode_entry : public log_entry {

public:

    mode_entry(bool is_autonomous, bool is_target);
    ~mode_entry();

    std::string get_name();

private:

    static const std::string ENTRY_ID;
    static const std::string MODE_KEY;
    static const std::string AUTONOMOUS;
    static const std::string MANUAL;
    static const std::string GPS;
};


#endif //PROJECT_MODE_ENTRY_H
