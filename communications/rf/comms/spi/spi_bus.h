//
// Created by Liam Kelly on 2/10/19.
//

#ifndef PROJECT_SPI_H
#define PROJECT_SPI_H

#define SPI_CLOCK 1000000
#define DUMMY_BYTE 0xFF

#include <vector>
#include <stdint.h>

class spi_bus {

public:

    virtual ~spi_bus() {}

    virtual void write_read_bytes(uint8_t* write, uint8_t *read, int len) = 0;

};


#endif //PROJECT_SPI_H
