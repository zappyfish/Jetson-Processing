//
// Created by Liam Kelly on 11/7/18.
//

#ifndef NVIDIA_LOG_EVENT_H
#define NVIDIA_LOG_EVENT_H

#include <string>
#include <map> // must be ordered map for logging
#include "time_manager.h"

class log_entry {

public:

    log_entry();
    /**
     * This returns a string log line for saving log entries. Example:
     * "TEST_ENTRY a_x 10.7 a_y 5.0 a_z 2.3\n"
     * @return string log line
     */
    std::string get_log_line();

    /**
     * Must implement this method for every derived class
     * @return the name of the log entry
     */
    virtual std::string get_name();

protected:

    static const std::string ENTRY_ID;

    void add_data(const std::string &tag, const int val);

    void add_data(const std::string &tag, const std::string val);

    void add_data(const std::string &tag, const double val);

private:

    std::map<std::string, std::string> m_event_data;

    std::string m_timestamp;

};

#endif //NVIDIA_LOG_EVENT_H
