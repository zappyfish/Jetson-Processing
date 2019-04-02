//
// Created by Liam Kelly on 4/2/19.
//

#include "rf_24_handler.h"

rf24_handler::rf24_handler(nrf_handler::board_type board, nrf_handler::mode md, unsigned int ce_pin,
                           rf24_handler::rf_callback *callback) : m_callback(callback) {

    if (board == rf24_handler::board_type::jetson) {
        m_rf_radio = new radio(ce_pin, 0);
    } else if (board == rf24_handler::board_type::rpi) {
        m_rf_radio = new radio(ce_pin, 0);
    } else {
        m_rf_radio = nullptr;
    }

    if (m_rf_radio != nullptr) {
        init_radio();
    }

}

rf24_handler::~rf24_handler() {
    if (m_rf_radio != nullptr) {
        delete m_rf_radio;
    }
}

void rf24_handler::check_packets() {

}

void rf24_handler::resend_last_packet() {}

void rf24_handler::send_packet(rf_packet &packet) {}

void rf24_handler::verify_spi() {}

void rf24_handler::set_mode(rf24_handler::mode md) {}

void rf24_handler::init_radio() {

}