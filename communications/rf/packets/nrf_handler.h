//
// Created by Liam Kelly on 2/9/19.
//

#ifndef PROJECT_NRF_HANDLER_H
#define PROJECT_NRF_HANDLER_H

#include <vector>
#include <chrono>
#include <thread>
#include "rf_packet.h"
#include "spi_bus.h"
#include "rpi_spi.h"
#include "uav_gpio.h"
#include "jetson_spi.h"
#include "rpi_gpio.h"
#include "jetson_gpio.h"
#include "dummy_spi.h"
#include "dummy_gpio.h"

#define REGISTER_MASK 0b00011111 // AND this w/ addresses for config registers
#define W_MASK 0b00100000 // OR this w/ masked addresses
#define R_MASK 0x00 // OR this w/ masked addresses

// spi command for reading/writing payloads
#define R_RX_PAYLOAD 0b01100001
#define W_TX_PAYLOAD 0b10100000

// addresses for registers that we care about
#define CONFIG 0x00
#define EN_AA 0x01
#define EN_RX_ADDR 0x02
#define RX_PW_P0 0x11 // the number of bytes available in RX
#define SETUP_AW 0x03
#define STATUS 0x07

#define READ_MODE 0x01
#define WRITE_MODE 0x00

#define CONFIG_BYTE 0b00001010

// pipe size
#define PIPE_SIZE 32

class nrf_handler {

public:

    static const int TAG_LENGTH;
    static const char* TAG;
    static const int CE_PULSE_TIME;

    enum board_type {
        dummy = 2,
        jetson = 1,
        rpi = 0,
    };

    enum mode {
        TX = 1,
        RX = 0,
    };

    nrf_handler(nrf_handler::board_type board, nrf_handler::mode md, unsigned int ce_pin, void(*callback)(rf_packet));
    ~nrf_handler();

    void check_packets();

    void send_packet(rf_packet &packet);

    void set_mode(mode md);

private:

    spi_bus* m_spi;
    uav_gpio* m_ce;


    uint8_t m_send_buf[PIPE_SIZE];
    uint8_t m_dummy_buf[PIPE_SIZE];
    nrf_handler::mode m_mode;

    void pulse_CE();
    void send_data(uint8_t* buf, int len);
    void reset_irq();
    void flush_tx();
    uint8_t get_bytes_available();

    bool data_available();
    void verify_spi();

    void (*m_callback)(rf_packet);
};


#endif //PROJECT_NRF_HANDLER_H
