//
// Created by Liam Kelly on 3/21/19.
//

#include "final_rpi_system.h"

final_rpi_system::final_rpi_system() : m_is_armed(false), m_lpf_accel(1, 0.2), m_should_sample_accel(true),
                                       m_accel_thread(&final_rpi_system::sample_accel_thread, this),
                                       m_has_received_destination(false),
                                       m_nrf_handler(nrf_handler::board_type::rpi, nrf_handler::mode::RX, 25, &m_rf_callback) {
    m_nrf_handler.verify_spi();
    m_rf_callback.callback = &final_rpi_system::rf_callback;
}

final_rpi_system::~final_rpi_system() {
    m_should_sample_accel = false;
    m_accel_thread.join();
}

void final_rpi_system::setup_packet_manager() {
    // Set uart
    rpi_uart *uart = new rpi_uart(rpi_uart::UART_MAIN, 115200);
    packet_manager::get_instance().set_uart(uart);

    // Shouldn't set any other callbacks until takeoff
    m_arming_packet_callback.name = arming_packet::PACKET_NAME;
    m_arming_packet_callback.callback = &final_rpi_system::arming_callback;
    m_arming_packet_callback.args = this;

    packet_manager::get_instance().set_packet_callback(&m_arming_packet_callback);
}

void final_rpi_system::flight_setup() {
    // Setup filesystem
    rpi_file_manager* f_manager = new rpi_file_manager();
    data_logger::get_instance().start_flight_session(f_manager);

    // Flight packet callback for logging
    m_flight_data_callback.name = flight_packet::PACKET_NAME;
    m_flight_data_callback.callback = &final_rpi_system::flight_packet_callback;
    m_flight_data_callback.args = &m_last_accel;

    packet_manager::get_instance().set_packet_callback(&m_flight_data_callback);

    // Mode packet callback for logging
    m_mode_callback.name = mode_packet::PACKET_NAME;
    m_mode_callback.callback = &final_rpi_system::mode_packet_callback;
    packet_manager::get_instance().set_packet_callback(&m_mode_callback);

    // Last callback: beacon deployment
    m_beacon_deployed_callback.name = gps_values_packet::PACKET_NAME;
    m_beacon_deployed_callback.callback = &final_rpi_system::beacon_deployed_callback;
    packet_manager::get_instance().set_packet_callback(&m_beacon_deployed_callback);

    // Setup camera
    m_flight_camera.start_capture(&m_image_buffer);
}

void final_rpi_system::flight_teardown() {
    // Stop logging flight packets
    packet_manager::get_instance().remove_packet_callback(&m_flight_data_callback);
    // Stop logging mode packets
    packet_manager::get_instance().remove_packet_callback(&m_mode_callback);
    // Finish with this directory for logging
    data_logger::get_instance().end_flight_session();
    // Finish up capture
    m_flight_camera.end_capture();
    // Reset clock in case we want to take off again
    time_manager::get_instance().reset_clock();
}

void final_rpi_system::perform_flight() {
    // TODO: send GPS first, then search for target
    // send_gps();
    while (m_is_armed.load()) {
        m_nrf_handler.check_packets(); // See if we got GPS yet, if so, send it to the pixhawk
        image_ptr img = m_image_buffer.retrieve_image();
        data_logger::get_instance().log_image(img);
        cv::Mat copy = img->get_mat_copy();
        // TODO: Image processing here
        m_target_detector.send_for_processing(copy, img->get_timestamp());
    }
}

void final_rpi_system::perform_camera_transform(double *v) {}

bool final_rpi_system::should_takeoff() {
    return m_is_armed.load();
}

void final_rpi_system::send_gps() {
    m_gps_ack_callback.name = ack_packet::PACKET_NAME;
    m_gps_ack_callback.callback = &final_rpi_system::gps_ack_callback;
    packet_manager::get_instance().set_packet_callback(&m_gps_ack_callback);

    while (!m_gps_received_by_pixhawk.load()) {
        gps_values_packet *packet = new gps_values_packet(m_destination_x, m_destination_y);
        packet_manager::get_instance().send_packet(packet);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    packet_manager::get_instance().remove_packet_callback(&m_gps_ack_callback);
}

void final_rpi_system::sample_accel_thread() {
    while (m_should_sample_accel.load()) {
        Vector3 accel_vect;
        m_lpf_accel.get_accel(accel_vect);
        m_last_accel.copy_vals(accel_vect);
        std::this_thread::sleep_for(std::chrono::milliseconds(3)); // sample at ~300hz
    }
}

void final_rpi_system::arming_callback(const char *packet_type, std::vector<const char *> keys, std::vector<const char *> values, void *args) {
    final_rpi_system* system = static_cast<final_rpi_system*>(args);

    arming_packet packet(keys, values);

    // Save this to the logs
    arming_entry* arming_action_log_entry = new arming_entry(packet);
    data_logger::get_instance().save_log_entry(arming_action_log_entry);

    system->m_is_armed = packet.should_be_armed();

    // Now send a response to acknowledge we've started take-off:
    ack_packet* acknowledgement_response = new ack_packet();
    packet_manager::get_instance().send_packet(acknowledgement_response);
}

void final_rpi_system::flight_packet_callback(const char *name, std::vector<const char *> keys, std::vector <const char *> values, void *args) {
    Vector3 *last_accel = static_cast<Vector3*>(args);
    Vector3 accel_vect;
    accel_vect.copy_vals(*last_accel);
    flight_packet packet(keys, values);
    pixhawk_entry *entry = new pixhawk_entry(packet, accel_vect);
    data_logger::get_instance().save_log_entry(entry);
}

void final_rpi_system::mode_packet_callback(const char *name, std::vector<const char *> keys,
                                            std::vector<const char *> values, void *args) {
    mode_packet packet(keys, values);
    mode_entry* entry = new mode_entry(packet.get_is_autonomous(), packet.get_is_target());
    data_logger::get_instance().save_log_entry(entry);
}

void final_rpi_system::gps_ack_callback(const char *name, std::vector<const char *> keys,
                                        std::vector<const char *> values, void *args) {
    // m_gps_received_by_pixhawk = true;
    // TODO: complete me
}

void final_rpi_system::rf_callback(rf_packet packet, void*args) {
//    final_rpi_system* system = static_cast<final_rpi_system*>(args);
//    system->m_destination_x = packet.get_gps_x();
//    system->m_destination_y = packet.get_gps_y();
//    system->m_has_received_destination = true;
//    system->m_gps_received_by_pixhawk = false;
    std::cout << "informing pixhawk of destination\n";
    gps_values_packet *send_packet = new gps_values_packet(packet.get_gps_x(), packet.get_gps_y());
    rf_gps_entry *gps_log = new rf_gps_entry(packet.get_gps_x(), packet.get_gps_y());
    data_logger::get_instance().save_log_entry(gps_log);
    packet_manager::get_instance().send_packet(send_packet); // Should probably do ack stuff here but....
}

void final_rpi_system::beacon_deployed_callback(const char *name, std::vector<const char *> keys,
                                                std::vector<const char *> values, void *args) {
    gps_values_packet packet(keys, values);
    gps_entry* gps_log = new gps_entry(packet.get_x(), packet.get_y());
    data_logger::get_instance().save_log_entry(gps_log);
}