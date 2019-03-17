//
// Created by Liam Kelly on 2/10/19.
//

#ifndef PROJECT_RPI_GPIO_H
#define PROJECT_RPI_GPIO_H

#ifdef __linux__

#include "gpio.h"
#include "libsoc_gpio.h"

#define RPI_SYSFS_GPIO_DIR "/sys/class/gpio"
#define RPI_MAX_BUF 64

class rpi_gpio : public gpio {

public:

    rpi_gpio(unsigned int pin_num, direction dir);
    ~rpi_gpio();

    void set_pin_state(state ste);
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


#endif //PROJECT_RPI_GPIO_H
