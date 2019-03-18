//
// Created by Liam Kelly on 3/13/19.
//

#include "uav_gpio.h"
#include "spi_bus.h"
#include "nrf_handler.h"
#include "rf_packet.h"
#include "rpi_file_manager.h"

void callback(rf_packet packet);

int main() {

    rpi_file_manager *f_manager = new rpi_file_manager();
    data_logger::get_instance().start_flight_session(f_manager);

    nrf_handler handler(nrf_handler::board_type::jetson, nrf_handler::mode::TX, 254, &callback);

    while (true) {
        handler.verify_spi();
    }

    rf_packet test_packet(-5, 1);

    handler.send_packet(test_packet);

    handler.set_mode(nrf_handler::mode::TX);

    handler.check_packets();

}

void callback(rf_packet packet) {
    std::cout << "callback on a packet\n";
    std::cout << packet.get_gps_x() << ", " << packet.get_gps_y() << std::endl;
}