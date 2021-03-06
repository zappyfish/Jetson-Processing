//
// Created by Liam Kelly on 2/9/19.
//

#include "nrf_handler.h"
#include <iostream>


const int nrf_handler::TAG_LENGTH = 4;
const char* nrf_handler::TAG = "vadl";
const int nrf_handler::CE_PULSE_TIME = 1; // pulse for 1 ms to transmit packets

nrf_handler::nrf_handler(nrf_handler::board_type board, nrf_handler::mode md, unsigned int ce_pin, rf_callback *callback) :
m_mode(md), address {0xAB, 0xCD, 0xAB, 0xCD, 0x88} {
    if (board == jetson) {
#ifdef __linux__
        // TODO: create jetson_spi
        m_spi = new jetson_spi(jetson_spi::SPI_DEVICE_DEFAULT, jetson_spi::CHIP_SELECT_DEFAULT);
        m_ce = new jetson_gpio(ce_pin, uav_gpio::direction::output);
#endif
    } else if (board == rpi) {
#ifdef __linux__
        m_spi = new rpi_spi(rpi_spi::SPI_DEVICE_DEFAULT, rpi_spi::CHIP_SELECT_DEFAULT);
        m_ce = new rpi_gpio(ce_pin, uav_gpio::direction::output);
#endif
    } else if (board == dummy){
        m_spi = new dummy_spi();
        m_ce = new dummy_gpio();
    } else {
        // TODO: log soft error? this should be impossible i think but wut is c++
    }
    m_send_buf[0] = W_TX_PAYLOAD;
    for (size_t i = 1; i <= TAG_LENGTH; i++) {
        m_send_buf[i] = TAG[i - 1];
    }
    m_callback = callback;
    init();
    flush_rx();
    reset_irq();
    set_mode(m_mode);
}

nrf_handler::~nrf_handler() {
    if (m_spi != nullptr) {
        delete m_spi;
    }
    if (m_ce != nullptr) {
        delete m_ce;
    }
}

void nrf_handler::check_packets() {
    if (m_callback == nullptr) {
        return; // For TX only systems
    }
    if (m_mode != mode::RX) {
        set_mode(mode::RX); // Won't have anything, so end here
    } else {
        m_ce->set_pin_state(uav_gpio::state::low); // standby mode so we can read registers
        if (data_available()) {
            uint8_t write[1 + PIPE_SIZE];
            write[0] = R_RX_PAYLOAD;
            for(uint8_t i = 1; i < 1 + PIPE_SIZE; ++i) {
                write[i] = 0xff; // dummy
            }

            uint8_t bytes_available = get_bytes_available();

            uint8_t read_buf[1 + PIPE_SIZE];
            m_spi->write_read_bytes(write, read_buf, bytes_available);

            reset_irq();

            // first, verify that it is PRIMO VADL CERTIFIED:
            for (int i = 1; i < 5; i++) {
                if (read_buf[i] != nrf_handler::TAG[i - 1]) {
                    m_ce->set_pin_state(uav_gpio::state::high); // go back to listening
                    return;
                }
            }
            uint8_t checksum = 0;
            for (int i = 5; i < 13; i++) {
                checksum = checksum ^ read_buf[i];
            }

            if (checksum != read_buf[13]) {
                m_ce->set_pin_state(uav_gpio::state::high); // go back to listening
                return; // checksum failure
            }

            // Create rf_packet, invoke callback
            rf_packet packet(&(read_buf[5]), bytes_available);

            m_callback->callback(packet, m_callback->args);
        }
        m_ce->set_pin_state(uav_gpio::state::high); // go back to listening
    }
}

void nrf_handler::send_packet(rf_packet &packet) {
    // Check packet size btw
    vector<uint8_t> serialized = packet.serialize();
    uint16_t len = serialized.size();
    if (len + TAG_LENGTH < PIPE_SIZE) {
        m_ce->set_pin_state(uav_gpio::state::low); // TODO: First go low so you can write to reg
        uint8_t checksum = 0;
        for (uint16_t i = 1; i <= len; i++) {
            m_send_buf[i + TAG_LENGTH] = serialized.at(i - 1);
        }
        // TODO: put in TX mode
        if (m_mode != mode::TX) {
            set_mode(mode::TX);
        }
        // TODO: send message
        send_data(m_send_buf, PIPE_SIZE + 1);

        // TODO: pulse CE
        // pulse_CE();

        // TODO: set TX high to transmit
        m_ce->set_pin_state(uav_gpio::state::high);
    }
}

void nrf_handler::init() {
    m_ce->set_pin_state(uav_gpio::state::low); // standby mode so we can write registers

    write_reg(EN_AA, 0x01); // enable auto-ack on pipe 0
    write_reg(EN_RX_ADDR, 0x01); // enable pipe 0
    write_reg(RX_PW_P0, PIPE_SIZE);
    write_reg(RETRY, 0xff); // max retries
    write_reg(SETUP_AW, 3);
    write_reg(RF_SETUP, 0x07);
    write_reg(RF_CH, 69);

    uint8_t rxaddr[6];
    uint8_t d[6];

    rxaddr[0] = (RX_ADDR_P0 & REGISTER_MASK) | W_MASK;
    for (uint8_t i = 0; i < 5; i++) {
        rxaddr[i + 1] = address[i];
    }
    m_spi->write_read_bytes(rxaddr, d, 6);

//    config_write[0] = (EN_AA & REGISTER_MASK) | W_MASK;
//    config_write[1] = 0x01;
//    m_spi->write_read_bytes(config_write, dummy_read, 2); // enable auto-ack on pipe 0
//
//    config_write[0] = (EN_RX_ADDR & REGISTER_MASK) | W_MASK;
//    config_write[1] = 0x01; // enable pipe 0
//    m_spi->write_read_bytes(config_write, dummy_read, 2);
//
//    config_write[0] = (RX_PW_P0 & REGISTER_MASK) | W_MASK;
//    config_write[1] = PIPE_SIZE;
//    m_spi->write_read_bytes(config_write, dummy_read, 2); // PIPE_SIZE bytes in rx payload
//
//    config_write[0] = (RETRY & REGISTER_MASK) | W_MASK;
//    config_write[1] = 0xff;
//    m_spi->write_read_bytes(config_write, dummy_read, 2);
//
//    config_write[0] = (SETUP_AW & REGISTER_MASK) | W_MASK;
//    config_write[1] = 3; // TODO: make this clearer later
//    m_spi->write_read_bytes(config_write, dummy_read, 2);
//
//    config_write[0] = (RF_SETUP & REGISTER_MASK) | W_MASK;
//    config_write[1] = 0x07; // TODO: make this clearer later
//    m_spi->write_read_bytes(config_write, dummy_read, 2);

    reset_irq();
}

void nrf_handler::write_reg(uint8_t reg, uint8_t val) {
    uint8_t dummy_read[2];
    uint8_t write[2];
    write[0] = (reg & REGISTER_MASK) | W_MASK;
    write[1] = val;
    m_spi->write_read_bytes(write, dummy_read, 2);
}

void nrf_handler::set_mode(nrf_handler::mode md) {
    // SPI stuff
    m_ce->set_pin_state(uav_gpio::state::low); // standby mode so we can write registers
    std::this_thread::sleep_for(std::chrono::milliseconds(nrf_handler::CE_PULSE_TIME)); // let it settle
    if (md == nrf_handler::mode::RX) {
        uint8_t rx_mode[2];
        rx_mode[0] = (CONFIG & REGISTER_MASK) | W_MASK;
        uint8_t config_byte = 0x0B;
        rx_mode[1] = config_byte; // TODO: make clearer
        uint8_t dummy_read[2];
        m_spi->write_read_bytes(rx_mode, dummy_read, 2);


        // Set RX ADDR

        std::cout << "setting into rx mode \n";

        verify_spi();

        reset_irq();
        m_ce->set_pin_state(uav_gpio::state::high); // Important!


    } else if (md == nrf_handler::mode::TX) {
        uint8_t tx_mode[2];
        tx_mode[0] = (CONFIG & REGISTER_MASK) | W_MASK;
        uint8_t config_byte = 0x0A;
        tx_mode[1] = config_byte; // make this clearer later
        uint8_t dummy_read[2];
        m_spi->write_read_bytes(tx_mode, dummy_read, 2);

        // Set TX ADDR
        uint8_t tx_addr_set[6];
        uint8_t d[6];
        tx_addr_set[0] = (TX_ADDR & REGISTER_MASK) | W_MASK;
        for (uint8_t i = 0; i < 5; i++) {
            tx_addr_set[i + 1] = address[i];
        }
        m_spi->write_read_bytes(tx_addr_set, d, 6); // This is disgusting
    }

    m_mode = md;
}

void nrf_handler::pulse_CE() {
    m_ce->set_pin_state(uav_gpio::state::high);
    std::this_thread::sleep_for(std::chrono::milliseconds(nrf_handler::CE_PULSE_TIME));
    m_ce->set_pin_state(uav_gpio::state::low);
}

void nrf_handler::reset_irq() {
    uint8_t write[2];
    write[0] = (STATUS & REGISTER_MASK) | W_MASK;
    write[1] = 0b01110000;
    uint8_t read[2];
    m_spi->write_read_bytes(write, read, 2);
}

bool nrf_handler::data_available() {
    uint8_t write[2];
    write[0] = (STATUS & REGISTER_MASK) | R_MASK;
    write[1] = 0xff; // dummy byte
    uint8_t read[2];
    m_spi->write_read_bytes(write, read, 2);
    uint8_t rx_interrupt = (1 << 6);
    // verify_spi();
    return (rx_interrupt & read[1]) == rx_interrupt;
}

void nrf_handler::send_data(uint8_t *buf, int len) {
    m_spi->write_read_bytes(buf, m_dummy_buf, len);
}

void nrf_handler::flush_tx() {
    uint8_t wrt[1];
    uint8_t dummy[1];
    wrt[0] = 0b11100001;
    m_spi->write_read_bytes(wrt, dummy, 1);
}

void nrf_handler::flush_rx() {
    uint8_t wrt[1];
    uint8_t dummy[1];
    wrt[0] = 0b11100010;
    m_spi->write_read_bytes(wrt, dummy, 1);
}

void nrf_handler::verify_spi() {
    uint8_t test[2];
    test[0] = (CONFIG & REGISTER_MASK) | R_MASK;
    test[1] = 0xff;
    uint8_t dat[2];
    m_spi->write_read_bytes(test, dat, 2);
    std::cout << "read: " << unsigned(dat[1]) << std::endl;
}

uint8_t nrf_handler::get_bytes_available() {
//    uint8_t wrt[2];
//    wrt[0] = (RX_PW_P0 & REGISTER_MASK) | R_MASK;
//    wrt[1] = 0xff;
//    uint8_t dat[2];
//    m_spi->write_read_bytes(wrt, dat, 2);
//    return dat[1];
    return PIPE_SIZE; // TODO: see if we can change this. probably not tho
}

void nrf_handler::resend_last_packet() {
    reset_irq();
    uint8_t cmd[1];
    cmd[0] = REUSE_TX;
    uint8_t d;
    m_spi->write_read_bytes(cmd, &d, 1);
    pulse_CE();
}