//
// Created by Liam Kelly on 2/10/19.
//

#include "rpi_gpio.h"

#ifdef __linux__

rpi_gpio::rpi_gpio(unsigned int pin_num, direction dir) : m_pin_num(pin_num) {
    export_gpio(m_pin_num);
    set_pin_direction(dir);
}

rpi_gpio::~rpi_gpio() {
    unexport_gpio(m_pin_num);
}

void rpi_gpio::set_pin_state(uav_gpio::state ste) {
    char buf[RPI_MAX_BUF];
    char value[RPI_MAX_BUF];
    std::snprintf(value, sizeof(value), "/gpio%d/value", m_pin_num);

    int len = std::sprintf(buf, sizeof(buf), "%d", ste);
    if(!file_write(value, buf, len)) {
        log_pin_error(std::string("could not set gpio value"), std::string("set_pin_state"));
    }
}

uav_gpio::state rpi_gpio::read_pin_state() {
    char buf[RPI_MAX_BUF];
    char value[RPI_MAX_BUF];
    std::snprintf(value, sizeof(value), "/gpio%d/value", m_pin_num);

    char ch;

    int fd = std::open(buf, O_RDONLY);
    if (fd < 0) {
        log_pin_error(std::string("could not read gpio value"), std::string("read_pin_value"));
    }

    read(fd, &ch, 1);

    close(fd);

    if (ch != '0') {
        return uav_gpio::state::high;
    } else {
        return uav_gpio::state::low;
    }
}

void rpi_gpio::export_gpio(unsigned int pin_num) {
    int len;
    char buf[RPI_MAX_BUF];
    len = std::snprintf(buf, sizeof(buf), "%d", pin_num);

    char *path = "/export";
    if (!file_write(path, buf, len)) {
        log_pin_error(std::string("could not export gpio"), std::string("export_gpio"));
    }
}

void rpi_gpio::unexport_gpio(unsigned int pin_num) {
    int len;
    char buf[RPI_MAX_BUF];
    len = std::snprintf(buf, sizeof(buf), "%d", pin_num);

    char *path = "/unexport";
    if (!file_write(path, buf, len)) {
        log_pin_error(std::string("could not unexport gpio"), std::string("unexport_gpio"));
    }
}

void rpi_gpio::set_pin_direction(uav_gpio::direction dir) {
    int len;
    char buf[RPI_MAX_BUF];
    char direction[RPI_MAX_BUF];
    std::snprintf(direction, sizeof(direction), "/gpio%d/direction", m_pin_num)


    if (dir == input) {
        len = std::snprintf(buf, sizeof(buf), "in");
    } else if (dir == output) {
        len = std::snprintf(buf, sizeof(buf), "out");
    }

    if (!file_write(direction, buf, len)) {
        log_pin_error(std::string("could not set gpio direction"), std::string("set_pin_direction"));
    }
}

bool rpi_gpio::file_write(char *path, char* write_buf, int len) {
    int fd;

    char open_buf[JETSON_MAX_BUF];
    int len1 = sizeof(JETSON_SYSFS_GPIO_DIR);
    for(int i = 0; i < len1; i++) {
        open_buf[i] = JETSON_SYSFS_GPIO_DIR[i];
    }

    int len2 = sizeof(path);
    for (int i = len1; i < len1 + len2; i++) {
        open_buf[i] = path[i - len1];
    }

    fd = open(open_buf, O_WRONLY);
    if (fd < 0) {
        return false;
    }

    write(fd, write_buf, len);
    close(fd);
    return true;
}

void rpi_gpio::log_pin_error(std::string description, std::string method) {
    std::string category = "rpi_gpio";
    std::map<std::string, std::string> metadata;
    metadata["file"] = "rpi_gpio.cpp";
    metadata["method"] = method;
    soft_error_entry *s_error = new soft_error_entry(category, description, metadata);
    data_logger::get_instance().save_log_entry(s_error);
}

#endif