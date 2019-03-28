//
// Created by Liam Kelly on 2/10/19.
//

#ifdef __linux__

#include "jetson_spi.h"

const int jetson_spi::CHIP_SELECT_DEFAULT = 0;
const int jetson_spi::SPI_DEVICE_DEFAULT = 3;

jetson_spi::jetson_spi(int spi_device, int chip_select) {
    init_spi_bus(spi_device, chip_select);
    if (m_spi_bus == nullptr) {
//        // TODO: log soft error here
//        std::string category = "jetson_spi";
//        std::string description = "could not init spi bus in constructor";
//        std::map<std::string, std::string> metadata;
//        metadata["file"] = "jetson_spi.cpp";
//        metadata["method"] = "constructor";
//        soft_error_entry *s_error = new soft_error_entry(category, description, metadata);
//        data_logger::get_instance().save_log_entry(s_error);
    }
}

jetson_spi::~jetson_spi() {
    libsoc_spi_free(m_spi_bus);
}

void jetson_spi::write_read_bytes(uint8_t *write, uint8_t *read, int len) {
    if (m_spi_bus != nullptr) {
        libsoc_spi_rw(m_spi_bus, write, read, len);
    }
}

void jetson_spi::init_spi_bus(int spi_device, int chip_select) {
    m_spi_bus = libsoc_spi_init(spi_device, chip_select);
    libsoc_spi_set_mode(m_spi_bus, MODE_0);
    libsoc_spi_set_speed(m_spi_bus, SPI_CLOCK);
    libsoc_spi_set_bits_per_word(m_spi_bus, BITS_8);
}

#endif