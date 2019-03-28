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
#include "packet_manager.h"
#include "gps_values_packet.h"

void callback(rf_packet packet, void *args);
void test_beacon_deployed_callback(const char *name, std::vector<const char *> keys,
                                   std::vector<const char *> values, void *args);

int main() {


    nrf_handler::rf_callback cb;

    nrf_handler handler(nrf_handler::board_type::rpi, nrf_handler::mode::TX, 25, &cb);

    cb.callback = &callback;


    handler.verify_spi();


    // Last callback: beacon deployment
    packet_manager::packet_callback gps_callback;
    gps_callback.name = gps_values_packet::PACKET_NAME;
    gps_callback.callback = &test_beacon_deployed_callback;
    gps_callback.args = &handler;
    packet_manager::get_instance().set_packet_callback(&gps_callback);

//    rf_packet test_packet(-5, 1);
//
//    handler.send_packet(test_packet);

    std::cout << "starting loop\n";

    while (true) {
        // handler.check_packets();
        packet_manager::get_instance().check_packets();
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // let it settle
        handler.resend_last_packet(); // Should be ok thread wiise b/c callback completes first
        // handler.resend_last_packet();
    }

    handler.set_mode(nrf_handler::mode::RX);

    handler.check_packets();

}

void callback(rf_packet packet, void *args) {
    std::cout << "callback on a packet\n";
    std::cout << packet.get_gps_x() << ", " << packet.get_gps_y() << std::endl;
}

void test_beacon_deployed_callback(const char *name, std::vector<const char *> keys,
                                                std::vector<const char *> values, void *args) {
    gps_values_packet packet(keys, values);
//    gps_entry* gps_log = new gps_entry(packet.get_x(), packet.get_y());
//    data_logger::get_instance().save_log_entry(gps_log);

    std::cout << "got location data\n";

    nrf_handler *handler = static_cast<nrf_handler*>(args);
    rf_packet rf(packet.get_x(), packet.get_y());
    handler->send_packet(rf);
}