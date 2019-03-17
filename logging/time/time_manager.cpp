//
// Created by Liam Kelly on 11/9/18.
//

#include "time_manager.h"

time_manager& time_manager::get_instance() {
    static time_manager instance;
    return instance;
}

time_manager::time_manager() : m_start_time(std::chrono::system_clock::now()) {
}

time_manager::~time_manager() {}

std::string time_manager::get_relative_time() {
    auto t_now = std::chrono::system_clock::now();
    auto time_millis = std::chrono::duration_cast<std::chrono::milliseconds>(t_now - m_start_time).count();
    std::string time_stamp(std::to_string(time_millis));
    return time_stamp;
}

void time_manager::reset_clock() {
    m_start_time = std::chrono::system_clock::now();
}