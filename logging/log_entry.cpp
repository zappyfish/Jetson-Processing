//
// Created by Liam Kelly on 11/7/18.
//

#include "log_entry.h"

const std::string log_entry::ENTRY_ID = "BASE_CLASS";

log_entry::log_entry() : m_timestamp(time_manager::get_instance().get_relative_time()) {}

std::string log_entry::get_log_line() {
    std::string line = m_timestamp;
    for (auto const &data_pair : m_event_data) {
        line += "," + data_pair.first + "," + data_pair.second;
    }
    line += "\n";
    return line;
}

void log_entry::add_data(const std::string &tag, const int val) {
    m_event_data[tag] = std::to_string(val);
}

void log_entry::add_data(const std::string &tag, const std::string val) {
    m_event_data[tag] = std::string(val);
}

void log_entry::add_data(const std::string &tag, const double val) {
    m_event_data[tag] = std::to_string(val);
}

std::string log_entry::get_name() { return log_entry::ENTRY_ID; }