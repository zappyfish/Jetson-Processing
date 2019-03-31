//
// Created by Liam Kelly on 3/21/19.
//

#ifndef PROJECT_FINAL_RPI_SYSTEM_H
#define PROJECT_FINAL_RPI_SYSTEM_H

#include "flight_system.h"
#include "raspicamera.h"
#include "rpi_file_manager.h"
#include "image_buffer.h"
#include "arming_packet.h"
#include "flight_packet.h"
#include "pixhawk_packet.h"
#include "pixhawk_entry.h"
#include "rpi_uart.h"
#include "packet_manager.h"
#include "arming_entry.h"
#include "ack_packet.h"
#include "target_processor.h"
#include "gps_values_packet.h"
#include "mode_entry.h"
#include "mode_packet.h"
#include "accel_mma_8451_pi.h"
#include "nrf_handler.h"
#include "gps_entry.h"
#include "rf_gps_entry.h"
#include "rf_packet.h"
#include <atomic>
#include <chrono>
#include <thread>

class final_rpi_system : public flight_system {

public:

    final_rpi_system();
    ~final_rpi_system();

    bool should_takeoff();

private:

    void setup_packet_manager();

    void setup_data_logger();

    void perform_camera_transform(double *v);

    void perform_flight();

    void flight_setup();

    void flight_teardown();

    void sample_accel_thread();

    void send_gps();

    // UART Callbacks
    static void arming_callback(const char *packet_type, std::vector<const char *> keys, std::vector<const char *> values, void *args);
    static void flight_packet_callback(const char *name, std::vector<const char *> keys, std::vector <const char *> values, void *args);
    static void mode_packet_callback(const char *name, std::vector<const char *> keys, std::vector <const char *> values, void *args);
    static void gps_ack_callback(const char *name, std::vector<const char *> keys, std::vector <const char *> values, void *args);
    static void gps_received_callback(const char *name, std::vector<const char *> keys, std::vector <const char *> values, void *args);

    // RF Callback
    static void rf_callback(rf_packet packet, void *args);

    /**
     * Member variables
     */
    raspicamera m_flight_camera;
    image_buffer m_image_buffer;
    target_processor m_target_detector;

    nrf_handler::rf_callback m_rf_callback;
    nrf_handler m_nrf_handler;

    std::atomic<bool> m_is_armed;
    std::atomic<bool> m_should_sample_accel;
    accel_mma8451_pi m_lpf_accel;
    Vector3 m_last_accel;
    std::thread m_accel_thread;

    int m_destination_x;
    int m_destination_y;
    std::atomic<bool> m_gps_received_by_pixhawk;
    std::atomic<bool> m_has_received_destination;

    packet_manager::packet_callback m_arming_packet_callback;
    packet_manager::packet_callback m_flight_data_callback;
    packet_manager::packet_callback m_mode_callback;
    packet_manager::packet_callback m_gps_ack_callback;

    packet_manager::packet_callback m_gps_received_callback;

};


#endif //PROJECT_FINAL_RPI_SYSTEM_H
