//
// Created by Liam Kelly on 11/16/18.
//

#if !defined(__linux__) && !defined(_WIN32) && !defined(__APPLE__)

#include "pixhawk_uart.h"

pixhawk_uart::pixhawk_uart() : hal(AP_HAL::get_HAL()) {
    hal.uartD->begin(115200);
}

pixhawk_uart::~pixhawk_uart() {}

void pixhawk_uart::read_buffer(std::vector<char> &read_data) {
    int nbytes = hal.uartD->available();
    for (int i = 0; i < nbytes; i++) {
        read_data.push_back(hal.uartD->read());
    }
}

void pixhawk_uart::write_buffer(std::vector<char> &data) {
    hal.uartD->write(reinterpret_cast<char*>(data.data()), data.size());
}

#endif