//
// Created by Liam Kelly on 1/26/19.
//

#ifndef PROJECT_JETSON_UART_H
#define PROJECT_JETSON_UART_H


#ifdef __linux__

#include "uart.h"
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

class jetson_uart : public uart {


public:

    static const char* UART_MAIN;
    static const speed_t DEFAULT_SPEED;

    jetson_uart(const char* device, speed_t baud_rate);
    ~jetson_uart();

    void write_buffer(std::vector<char> &data);

    void read_buffer(std::vector <char> &read_data);

private:

    int m_fd;

    bool has_uart();
};

#endif

#endif //PROJECT_JETSON_UART_H
