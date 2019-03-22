//
// Created by Liam Kelly on 1/26/19.
//

#include "jetson_uart.h"
#include <cstring>

#ifdef __linux__

#include "soft_error_entry.h"
#include "data_logger.h"
#include <iostream>

const char* jetson_uart::UART_MAIN = "/dev/ttyTHS1";
const speed_t jetson_uart::DEFAULT_SPEED = B115200;

jetson_uart::jetson_uart(const char* device, speed_t baud_rate) {
    m_fd = open(device, O_RDWR | O_NOCTTY);

    struct termios tty;
    std::memset(&tty, 0, sizeof tty);
    if (tcgetattr(m_fd, &tty) != 0)
    {
           // TODO: log soft error
           std::string category = "jetson uart";
            std::string description = "could not open uart";
            std::map<std::string, std::string> metadata;
            metadata["file"] = "jetson_uart.cpp";
            metadata["method"] = "constructor";
            soft_error_entry *s_error = new soft_error_entry(category, description, metadata);
            data_logger::get_instance().save_log_entry(s_error);
    }

    cfsetospeed(&tty, baud_rate);
    cfsetispeed(&tty, baud_rate);

//    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
//    // disable IGNBRK for mismatched speed tests; otherwise receive break
//    // as \000 chars
//    tty.c_iflag &= ~IGNBRK;         // disable break processing
//    tty.c_lflag = 0;                // no signaling chars, no echo,
//                                    // no canonical processing
//    tty.c_oflag = 0;                // no remapping, no delays
//    tty.c_cc[VMIN]  = 0;            // read doesn't block
//    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout
//
//    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl
//
//    tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
//                                    // enable reading
//    tty.c_cflag &= ~PARENB;      // shut off parity
//    tty.c_cflag &= ~CSTOPB;
//    tty.c_cflag &= ~CRTSCTS;

    cfmakeraw(&tty);

    if (tcsetattr(m_fd, TCSANOW, &tty) != 0) {
        // TODO: log soft error

    }
}

jetson_uart::~jetson_uart() {
    if (has_uart()) {
        close(m_fd);
    }
}

void jetson_uart::write_buffer(std::vector<char> &data) {
    if (has_uart()) {
        write(m_fd, data.data(), data.size());
    }
}

void jetson_uart::read_buffer(std::vector<char> &read_data) {
    if (has_uart()) {
        char buf[50]; // read 50 at a time
        int bytes_read = read(m_fd, buf, 50);
        while (bytes_read > 0) {
            for (int i = 0; i < bytes_read; i++) {
                read_data.push_back(buf[i]);
            }
            bytes_read = read(m_fd, buf, 50);
        }
    }
}

bool jetson_uart::has_uart() {
    return m_fd != -1;
}

#endif