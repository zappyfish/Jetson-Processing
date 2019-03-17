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

void callback(rf_packet packet);

int main() {

    nrf_handler handler(nrf_handler::board_type::rpi, nrf_handler::mode::TX, 8, &callback);

    handler.verify_spi();

    rf_packet test_packet(-5, 1);

    handler.send_packet(test_packet);

    handler.set_mode(nrf_handler::mode::TX);

    handler.check_packets();

}

void callback(rf_packet packet) {
    std::cout << "callback on a packet\n";
    std::cout << packet.get_gps_x() << ", " << packet.get_gps_y() << std::endl;
}