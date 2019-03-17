//
// Created by Liam Kelly on 11/9/18.
//

#ifndef NVIDIA_TIME_MANAGER_H
#define NVIDIA_TIME_MANAGER_H

#include <chrono>
#include <cstddef>
#include <string>
#include <sstream>
#include <time.h>

using namespace std::chrono;

class time_manager {

public:
    ~time_manager();

    std::string get_relative_time();
    /**
     * Should we have this method? Not sure if it's safe
     */
    void reset_clock();

    static time_manager& get_instance();

private:
    time_manager();

    std::chrono::time_point<std::chrono::system_clock> m_start_time;
};


#endif //NVIDIA_TIME_MANAGER_H
