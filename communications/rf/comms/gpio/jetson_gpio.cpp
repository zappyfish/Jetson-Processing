//
// Created by Liam Kelly on 2/10/19.
//

#include "jetson_gpio.h"

#ifdef __linux__

jetson_gpio::jetson_gpio(unsigned int pin_num, direction dir) : m_pin_num(pin_num) {
    export_gpio(m_pin_num)
    set_pin_direction(dir)
}

jetson_gpio::~jetson_gpio() {
    unexport_gpio(m_pin_num);
}

void jetson_gpio::set_pin_state(gpio::state ste) {
    char buf[JETSON_MAX_BUF];
    char value[JETSON_MAX_BUF];
    std::snprintf(value, sizeof(value), "/gpio%d/value", m_pin_num);

    len = std::sprintf(buf, sizeof(buf), "%d", ste);
    if(!file_write(value, buf, len)) {
        log_pin_error(std::string("could not set gpio value"), std::string("set_pin_state"));
    }
}

state jetson_gpio::read_pin_state() {
    char buf[JETSON_MAX_BUF];
    std::snprintf(value, sizeof(value), "/gpio%d/value", m_pin_num);

    char ch;

    fd = open(buf, O_RDONLY);
    if (fd < 0) {
        log_pin_error(std::string("could not read gpio value"), std::string("read_pin_value"));
    }

    read(fd, &ch, 1);

    close(fd);

    if (ch != '0') {
        return high;
    } else {
        return low;
    }
}

void jetson_gpio::export_gpio(unsigned int pin_num) {
    int len;
    char buf[JETSON_MAX_BUF];
    len = std::snprintf(buf, sizeof(buf), "%d", pin_num);

    if (!file_write("/export", buf, len)) {
        log_pin_error(std::string("could not export gpio"), std::string("export_gpio"));
    }
}

void jetson_gpio::unexport_gpio(unsigned int pin_num) {
    int len;
    char buf[JETSON_MAX_BUF];
    len = std::snprintf(buf, sizeof(buf), "%d", pin_num);

    if (!file_write("/unexport", buf, len)) {
        log_pin_error(std::string("could not unexport gpio"), std::string("unexport_gpio"));
    }
}

void jetson_gpio::set_pin_direction(gpio::direction dir) {
    int len;
    char buf[JETSON_MAX_BUF];
    char direction[JETSON_MAX_BUF];
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

bool jetson_gpio::file_write(char *path, char* write_buf, int len) {
    int fd;

    fd = open(JETSON_SYSFS_GPIO_DIR path, O_WRONLY);
    if (fd < 0) {
        return fals;
    }

    write(fd, buf, len);
    close(fd);
    return true;
}

void jetson_gpio::log_pin_error(std::string description, std::string method) {
    std::string category = "jetson_gpio";
    std::map<std::string, std::string> metadata;
    metadata["file"] = "jetson_gpio.cpp";
    metadata["method"] = method;
    soft_error_entry *s_error = new soft_error_entry(category, description, metadata);
    data_logger::get_instance().save_log_entry(s_error);
}

#endif