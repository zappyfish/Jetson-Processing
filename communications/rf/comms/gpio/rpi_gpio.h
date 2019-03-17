//
// Created by Liam Kelly on 2/10/19.
//

#ifndef PROJECT_RPI_GPIO_H
#define PROJECT_RPI_GPIO_H

#ifdef __linux__

#include "uav_gpio.h"
#include "data_logger.h"
#include "soft_error_entry.h"
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>

#define RPI_SYSFS_GPIO_DIR "/sys/class/gpio"
#define RPI_MAX_BUF 64

class rpi_gpio : public uav_gpio {

public:

    rpi_gpio(unsigned int pin_num, direction dir);
    ~rpi_gpio();

    void set_pin_state(uav_gpio::state ste);
    state read_pin_state();

private:

    void export_gpio(unsigned int pin_num);
    void unexport_gpio(unsigned int pin_num);

    void set_pin_direction(direction dir);

    bool file_write(char const* path, char* write_buf, int len);

    void log_pin_error(std::string description, std::string method);

    unsigned int m_pin_num;
};

#endif


#endif //PROJECT_RPI_GPIO_H
