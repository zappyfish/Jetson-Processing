//
// Created by Liam Kelly on 2/10/19.
//

#include "rpi_spi.h"

#ifdef __linux__

const int rpi_spi::CHIP_SELECT_DEFAULT = 1;
const int rpi_spi::SPI_DEFICE_DEVAULT = 0;

rpi_spi::rpi_spi(int spi_device, int chip_select) {
    init_spi_bus(spi_device, chip_select);
    if (m_spi_bus == nullptr) {
        // TODO: log soft error here
        std::string category = "rpi_spi";
        std::string description = "could not init spi bus in constructor";
        std::map<std::string, std::string> metadata;
        metadata["file"] = "rpi_spi.cpp";
        metadata["method"] = "constructor";
        soft_error_entry *s_error = new soft_error_entry(category, description, metadata);
        data_logger::get_instance().save_log_entry(s_error);
    }
}

rpi_spi::~rpi_spi() {
    libsoc_spi_free(m_spi_bus)
}

void rpi_spi::write_read_bytes(uint8_t *write, uint8_t *read, int len) {
    if (m_spi_bus != nullptr) {
        libsoc_spi_rw(m_spi_bus, write, read, len);
    }
}

void rpi_spi::init_spi_bus(int spi_device, int chip_select) {
    m_spi_bus = libsoc_spi_init(spi_device, chip_select);
    libsoc_spi_set_mode(m_spi_bus, MODE_0);
    libsoc_spi_set_speed(m_spi_bus, SPI_CLOCK);
    libsoc_spi_set_bits_per_word(m_spi_bus, BITS_8);
}

#endif