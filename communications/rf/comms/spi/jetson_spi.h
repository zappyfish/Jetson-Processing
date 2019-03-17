//
// Created by Liam Kelly on 2/10/19.
//

#ifndef PROJECT_JETSON_SPI_H
#define PROJECT_JETSON_SPI_H

#ifdef __linux__

#include "spi_bus.h"
#include "libsoc.h"

class jetson_spi : public spi_bus {

public:

    jetson_spi();
    ~jetson_spi();

    void write_read_bytes(uint8_t* write, uint8_t *read, int len);

private:



};

#endif


#endif //PROJECT_JETSON_SPI_H
