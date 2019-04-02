//
// Created by Liam Kelly on 3/13/19.
//

#include "uav_gpio.h"
#include "spi_bus.h"
#include "nrf_handler.h"
#include <thread>
#include "rf_packet.h"

#include <RF24/RF24.h>

void callback(rf_packet packet, void *args);

int main() {

    nrf_handler::rf_callback cb;

    nrf_handler handler(nrf_handler::board_type::jetson, nrf_handler::mode::TX, 254, nullptr);

    cb.callback = &callback;

    handler.verify_spi();

    rf_packet test_packet(-5, 1);

    handler.send_packet(test_packet);


    while (true) {
        handler.resend_last_packet();

        // handler.check_packets();
    }

// Setup for GPIO 15 CE and CE0 CSN with SPI Speed @ 8Mhz
    RF24 radio(254,3);

    // Setup and configure rf radio
    radio.begin();

    // optionally, increase the delay between retries & # of retries
    radio.setRetries(15,15);
    // Dump the configuration of the rf unit for debugging
    radio.printDetails();

}

void callback(rf_packet packet, void *args) {
    std::cout << "callback on a packet\n";
    std::cout << packet.get_gps_x() << ", " << packet.get_gps_y() << std::endl;
}