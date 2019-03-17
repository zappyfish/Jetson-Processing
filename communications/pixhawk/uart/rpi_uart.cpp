//
// Created by Liam Kelly on 11/12/18.
//

#include "rpi_uart.h"
#include <iostream>

#ifdef __linux__

const char* rpi_uart::UART_MAIN = "/dev/serial0";

rpi_uart::rpi_uart(const char* device, int baud_rate) {
    m_fd = serialOpen(device, baud_rate);
}

rpi_uart::~rpi_uart() {
    if (has_uart()) {
        serialClose(m_fd);
    }
}

void rpi_uart::write_buffer(std::vector<char> &data) {
    if (has_uart()) {
        for (auto const &c: data) {
            serialPutchar(m_fd, c);
        }
    }
}

void rpi_uart::read_buffer(std::vector<char> &read_data) {
    if (has_uart()) {
        int chars_available = serialDataAvail(m_fd);
        for (int i = 0; i < chars_available; i++) {
            read_data.push_back(serialGetchar(m_fd));
        }
    }
}

bool rpi_uart::has_uart() {
    return m_fd != -1;
}

#endif