//
// Created by Liam Kelly on 3/13/19.
//

#ifndef PROJECT_DUMMY_SPI_H
#define PROJECT_DUMMY_SPI_H

#ifndef __linux__

#include "spi_bus.h"
#include <iostream>

class dummy_spi : public spi_bus {

public:

    dummy_spi() {

    }

    ~dummy_spi() {

    }

    void write_read_bytes(uint8_t* write, uint8_t *read, int len) {
        std::cout << "wr spi\n";
        dummy_buf[0] = write[0];
        dummy_buf[1] = write[1];
    }

private:

    uint8_t dummy_buf[2];
};

#endif

#endif //PROJECT_DUMMY_SPI_H
