//
// Created by Liam Kelly on 4/2/19.
//

#include "rf_24_handler.h"

const int rf24_handler::ADDRESS = 0xABCDABCD3fLL;

rf24_handler::rf24_handler(rf24_handler::board_type board, rf24_handler::mode md, unsigned int ce_pin,
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

void rf24_handler::resend_last_packet() {
    if (m_rf_radio != nullptr) {

    }
}

void rf24_handler::send_packet(rf_packet &packet) {
    if (m_rf_radio != nullptr) {

    }
}

void rf24_handler::verify_spi() {
    if (m_rf_radio != nullptr) {

    }
}

void rf24_handler::set_mode(rf24_handler::mode md) {
    if (m_rf_radio != nullptr) {

    }
}

void rf24_handler::init_radio() {
    if (m_rf_radio != nullptr) {
        m_rf_radio.begin();                           // Setup and configure rf radio
        m_rf_radio.setChannel(69);
        m_rf_radio.setPALevel(RF24_PA_MAX);
        m_rf_radio.setDataRate(RF24_1MBPS);
        m_rf_radio.setAutoAck(1);                     // Ensure autoACK is enabled
        m_rf_radio.setRetries(10,15);                  // Optionally, increase the delay between retries & # of retries
        m_rf_radio.setCRCLength(RF24_CRC_8);          // Use 8-bit CRC for performance
    }
}