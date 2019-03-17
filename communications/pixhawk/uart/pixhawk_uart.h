//
// Created by Liam Kelly on 11/16/18.
//

#ifndef ARDUCOPTER_PIXHAWK_UART_H
#define ARDUCOPTER_PIXHAWK_UART_H

#if !defined(__linux__) && !defined(_WIN32) && !defined(__APPLE__)

#include "uart.h"
#include "AP_HAL/AP_HAL.h"

class pixhawk_uart : public uart {

public:

    pixhawk_uart();
    ~pixhawk_uart();

    void write_buffer(std::vector<char> &data);
    void read_buffer(std::vector <char> &read_data);

private:

    const AP_HAL::HAL& hal;
};

#endif

#endif //ARDUCOPTER_PIXHAWK_UART_H
