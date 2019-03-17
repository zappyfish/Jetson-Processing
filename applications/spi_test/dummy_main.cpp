//
// Created by Liam Kelly on 3/13/19.
//

#include "gpio.h"
#include "spi_bus.h"
#include "nrf_handler.h"
#include "rf_packet.h"

void callback(rf_packet packet);

int main() {

    nrf_handler handler(nrf_handler::board_type::dummy, nrf_handler::mode::TX, 1, &callback);

    rf_packet test_packet(-5, 1);

    handler.send_packet(test_packet);

    handler.set_mode(nrf_handler::mode::TX);

    handler.check_packets();

}

void callback(rf_packet packet) {
    std::cout << "callback on a packet\n";
    std::cout << packet.get_gps_x() << ", " << packet.get_gps_y() << std::endl;
}