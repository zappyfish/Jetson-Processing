//
// Created by Liam Kelly on 3/13/19.
//

//
// Created by Liam Kelly on 3/13/19.
//

#include "uav_gpio.h"
#include "spi_bus.h"
#include "nrf_handler.h"
#include "rf_packet.h"
#include <RF24/RF24.h>

void callback(rf_packet packet, void *args);

int main() {


    nrf_handler::rf_callback cb;

    nrf_handler handler(nrf_handler::board_type::rpi, nrf_handler::mode::RX, 25, &cb);

    cb.callback = &callback;


    handler.verify_spi();

    std::cout << "starting loop\n";
//
////    rf_packet test_packet(-5, 1);
////
////    handler.send_packet(test_packet);
//
    while (true) {
        handler.check_packets();
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // let it settle

        // handler.resend_last_packet();
    }

//    handler.set_mode(nrf_handler::mode::RX);
//
//    handler.check_packets();

    RF24 radio(25,0);

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