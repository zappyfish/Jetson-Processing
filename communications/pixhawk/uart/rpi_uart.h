//
// Created by Liam Kelly on 11/12/18.
//

#ifndef NVIDIA_RPI_UART_H
#define NVIDIA_RPI_UART_H

#ifdef __linux__

#include <wiringSerial.h>
#include "uart.h"

class rpi_uart : public uart {

public:

    static const char* UART_MAIN;

    rpi_uart(const char* device, int baud_rate);
    ~rpi_uart();

    void write_buffer(std::vector<char> &data);

    void read_buffer(std::vector <char> &read_data);

private:

    int m_fd;

    bool has_uart();

};

#endif

#endif //NVIDIA_RPI_UART_H

