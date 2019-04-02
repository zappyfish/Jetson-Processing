//
// Created by Liam Kelly on 4/2/19.
//

#ifndef PROJECT_RF_24_HANDLER_H
#define PROJECT_RF_24_HANDLER_H

#include <RF24/RF24.h>
#include "rf_packet.h"

class rf24_handler {

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

    typedef struct {
        void (*callback)(rf_packet, void* args);
        void *args;
    } rf_callback;

    rf24_handler(nrf_handler::board_type board, nrf_handler::mode md, unsigned int ce_pin, rf_callback *callback);
    ~rf24_handler();

    void check_packets();

    void send_packet(rf_packet &packet);

    void resend_last_packet();

    void set_mode(mode md);

    void verify_spi();

private:

    radio* m_rf_radio;
    rf_callback *m_callback;

    void init_radio();
};


#endif //PROJECT_RF_24_HANDLER_H
