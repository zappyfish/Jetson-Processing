//
// Created by Liam Kelly on 11/8/18.
//

#ifndef NVIDIA_SOFT_ERROR_ENTRY_H
#define NVIDIA_SOFT_ERROR_ENTRY_H

#include "log_entry.h"

class soft_error_entry : public log_entry {

public:

    soft_error_entry(std::string category, std::string description, std::map <std::string, std::string> metadata);

    std::string get_name();

private:

    static const std::string ENTRY_ID;

};

#endif //NVIDIA_SOFT_ERROR_ENTRY_H
