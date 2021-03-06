//
// Created by Liam Kelly on 2/10/19.
//

#ifndef PROJECT_RPI_SPI_H
#define PROJECT_RPI_SPI_H

#ifdef __linux__

#include "spi_bus.h"
#include "libsoc_spi.h"
#include "data_logger.h"
#include "soft_error_entry.h"

class rpi_spi : public spi_bus {

public:

    static const int SPI_DEVICE_DEFAULT;
    static const int CHIP_SELECT_DEFAULT;

    rpi_spi(int spi_device, int chip_select);
    ~rpi_spi();

    void write_read_bytes(uint8_t* write, uint8_t *read, int len);

private:

    spi* m_spi_bus;

    void init_spi_bus(int spi_device, int chip_select);

};

#endif


#endif //PROJECT_RPI_SPI_H
