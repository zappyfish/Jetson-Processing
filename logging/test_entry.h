//
// Created by Liam Kelly on 11/7/18.
//

#ifndef NVIDIA_TEST_ENTRY_H
#define NVIDIA_TEST_ENTRY_H

#include "log_entry.h"

class test_entry : public log_entry {

public:

    test_entry(double a_x, double a_y, double a_z);

    std::string get_name();

private:

    static const std::string ENTRY_ID;

};

#endif //NVIDIA_TEST_ENTRY_H
