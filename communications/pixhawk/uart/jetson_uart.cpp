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
    m_fd = open(device, O_RDWR | O_NOCTTY | O_NONBLOCK);

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


    cfmakeraw(&tty);
    cfsetospeed(&tty, baud_rate);
    cfsetispeed(&tty, baud_rate);

    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE |
                   ECHOK | ECHONL |
                   ISIG | IEXTEN)
    tty.c_oflag &= ~(OPOST | ONLCR | OCRNL);
    tty.c_iflag &= ~(INLCR | IGNCR | ICRNL | IGNBRK);
    tty.c_iflag &= ~(INPCK | ISTRIP);
    tty.c_cflag &= ~(PARENB | PARODD | CMSPAR)

    int status;
    ioctl (m_fd, TIOCMGET, &status);

    status |= TIOCM_DTR;
    status |= TIOCM_RTS;

    ioctl (m_fd, TIOCMSET, &status);

    usleep (10000) ;	// 10mS

    if (tcsetattr(m_fd, TCSANOW, &tty) != 0) {
        // TODO: log soft error

    }

    sleep(2); //required to make flush work, for some reason
    tcflush(m_fd,TCIOFLUSH);
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