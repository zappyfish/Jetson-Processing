//
// Created by Liam Kelly on 2/10/19.
//

#ifndef PROJECT_JETSON_GPIO_H
#define PROJECT_JETSON_GPIO_H

#ifdef __linux__

#define JETSON_SYSFS_GPIO_DIR "/sys/class/gpio"
#define JETSON_MAX_BUF 64

#include "uav_gpio.h"
#include "data_logger.h"
#include "soft_error_entry.h"
#include <cstdio>

class jetson_gpio : public uav_gpio {

public:

    jetson_gpio(unsigned int pin_num, direction dir);
    ~jetson_gpio();

    void set_pin_state(uav_gpio::state ste);
    state read_pin_state();

private:

    void export_gpio(unsigned int pin_num);
    void unexport_gpio(unsigned int pin_num);

    void set_pin_direction(direction dir);

    bool file_write(char* path, char* write_buf, int len);

    void log_pin_error(std::string description, std::string method);

    unsigned int m_pin_num;
};

#endif


#endif //PROJECT_JETSON_GPIO_H
