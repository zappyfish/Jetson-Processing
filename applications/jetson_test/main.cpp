//
// Created by Liam Kelly on 11/15/18.
//

#include "image_buffer.h"
#include "jetson_camera.h"
#include "rpi_file_manager.h"
#include "data_logger.h"
#include "flight_packet.h"
#include "time_manager.h"
#include "pixhawk_entry.h"
#include "jetson_uart.h"
#include "packet_manager.h"
#include "test_uart.h"

void packet_logging_thread();
void flight_packet_callback(const char *name, std::vector<const char *> keys, std::vector <const char *> values, void *args);

int main() {
    size_t start_time = time_manager::get_instance().get_relative_time();
    rpi_file_manager *f_manager = new rpi_file_manager();
    data_logger::get_instance().start_flight_session(f_manager);

    packet_manager::packet_callback flight_data_callback;
    flight_data_callback.callback = &flight_packet_callback;
    flight_data_callback.name = flight_packet::PACKET_NAME;

    packet_manager::get_instance().set_packet_callback(&flight_data_callback);
    jetson_uart *uart = new jetson_uart(jetson_uart::UART_MAIN, jetson_uart::DEFAULT_SPEED);
    packet_manager::get_instance().set_uart(uart);

    image_buffer *main_buffer = new image_buffer();
    jetson_camera camera(jetson_camera::DEFAULT_WIDTH, jetson_camera::DEFAULT_HEIGHT, jetson_camera::DEFAULT_FPS);
    camera.start_capture(main_buffer);

    std::thread packet_thread(packet_logging_thread);

    while (true) {
        image_ptr img = main_buffer->retrieve_image();
        data_logger::get_instance().log_image(img);
    }

    // std::cout << "done\n";

    // return 0;
}

void packet_logging_thread() {
    while (true) {
        size_t num_packets;
        if ((num_packets = packet_manager::get_instance().check_packets())) {
            // std::cout << "there are " << num_packets << " packets\n";
        }
    }
}

void flight_packet_callback(const char *name, std::vector<const char *> keys, std::vector <const char *> values, void *args) {
    flight_packet packet(keys, values);
    pixhawk_entry *entry = new pixhawk_entry(packet);
    data_logger::get_instance().save_log_entry(entry);
}