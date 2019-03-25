//
// Created by Liam Kelly on 2/7/19.
//

#include "subscale_2_system.h"

subscale2_system::subscale2_system() : m_is_armed(false), m_lpf_accel(1, 0.05), m_should_sample_accel(true),
m_accel_thread(&subscale2_system::sample_accel_thread, this) {
}

subscale2_system::~subscale2_system() {
    m_should_sample_accel = false;
    m_accel_thread.join();
}

void subscale2_system::setup_packet_manager() {
    // Set uart
    rpi_uart *uart = new rpi_uart(rpi_uart::UART_MAIN, 115200);
    packet_manager::get_instance().set_uart(uart);

    // Shouldn't set any other callbacks until takeoff
    m_arming_packet_callback.name = arming_packet::PACKET_NAME;
    m_arming_packet_callback.callback = &subscale2_system::arming_callback;
    m_arming_packet_callback.args = this;

    packet_manager::get_instance().set_packet_callback(&m_arming_packet_callback);
}

void subscale2_system::flight_setup() {
    // Setup filesystem
    rpi_file_manager* f_manager = new rpi_file_manager();
    data_logger::get_instance().start_flight_session(f_manager);

    // Flight packet callback for logging
    m_flight_data_callback.name = flight_packet::PACKET_NAME;
    m_flight_data_callback.callback = &subscale2_system::flight_packet_callback;
    m_flight_data_callback.args = &m_last_accel;

    packet_manager::get_instance().set_packet_callback(&m_flight_data_callback);

    // Mode packet callback for logging
    m_mode_callback.name = mode_packet::PACKET_NAME;
    m_mode_callback.callback = &subscale2_system::mode_packet_callback;

    packet_manager::get_instance().set_packet_callback(&m_mode_callback);

    // Setup camera
    m_flight_camera.start_capture(&m_image_buffer);
}

void subscale2_system::flight_teardown() {
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

void subscale2_system::perform_flight() {
    double location[2];
    while (m_is_armed.load()) {
        image_ptr img = m_image_buffer.retrieve_image();
        data_logger::get_instance().log_image(img);
        cv::Mat copy = img->get_mat_copy();
        // TODO: Image processing here
        m_target_detector.send_for_processing(copy, img->get_timestamp());
    }
}

void subscale2_system::perform_camera_transform(double *v) {}

bool subscale2_system::should_takeoff() { return m_is_armed.load(); }

void subscale2_system::sample_accel_thread() {
    while (m_should_sample_accel.load()) {
        Vector3 accel_vect;
        m_lpf_accel.get_accel(accel_vect);
        m_last_accel.copy_vals(accel_vect);
        std::this_thread::sleep_for(std::chrono::milliseconds(3)); // sample at ~300hz
    }
}

void subscale2_system::arming_callback(const char *packet_type, std::vector<const char *> keys, std::vector<const char *> values, void *args) {
    subscale2_system* system = static_cast<subscale2_system*>(args);

    arming_packet packet(keys, values);

    // Save this to the logs
    arming_entry* arming_action_log_entry = new arming_entry(packet);
    data_logger::get_instance().save_log_entry(arming_action_log_entry);

    system->m_is_armed = packet.should_be_armed();

    // Now send a response to acknowledge we've started take-off:
    ack_packet* acknowledgement_response = new ack_packet();
    packet_manager::get_instance().send_packet(acknowledgement_response);
}

void subscale2_system::flight_packet_callback(const char *name, std::vector<const char *> keys, std::vector <const char *> values, void *args) {
    Vector3 *last_accel = static_cast<Vector3*>(args);
    Vector3 accel_vect;
    accel_vect.copy_vals(*last_accel);
    flight_packet packet(keys, values);
    pixhawk_entry *entry = new pixhawk_entry(packet, accel_vect);
    data_logger::get_instance().save_log_entry(entry);
}

void subscale2_system::mode_packet_callback(const char *name, std::vector<const char *> keys,
                                            std::vector<const char *> values, void *args) {
    mode_packet packet(keys, values);
    mode_entry* entry = new mode_entry(packet.get_is_autonomous());
    data_logger::get_instance().save_log_entry(entry);
}