//
// Created by Liam Kelly on 3/21/19.
//

#ifndef PROJECT_FINAL_JETSON_SYSTEM_H
#define PROJECT_FINAL_JETSON_SYSTEM_H

#include "flight_system.h"
#include "jetson_camera.h"
#include "jetson_file_manager.h"
#include "image_buffer.h"
#include "arming_packet.h"
#include "flight_packet.h"
#include "pixhawk_packet.h"
#include "pixhawk_entry.h"
#include "jetson_uart.h"
#include "packet_manager.h"
#include "gps_values_packet.h"
#include "arming_entry.h"
#include "ack_packet.h"
#include "target_processor.h"
#include "mode_entry.h"
#include "mode_packet.h"
#include "accel_mma_8451_pi.h"
#include "gps_entry.h"
#include "nrf_handler.h"
#include "rf_packet.h"
#include <atomic>
#include <chrono>
#include <thread>

class final_jetson_system : public flight_system {

public:

    final_jetson_system();
    ~final_jetson_system();

    bool should_takeoff();

private:

    void setup_packet_manager();

    void setup_data_logger();

    void perform_camera_transform(double *v);

    void perform_flight();

    void flight_setup();

    void flight_teardown();

    void sample_accel_thread();

    static void arming_callback(const char *packet_type, std::vector<const char *> keys, std::vector<const char *> values, void *args);
    static void flight_packet_callback(const char *name, std::vector<const char *> keys, std::vector <const char *> values, void *args);
    static void mode_packet_callback(const char *name, std::vector<const char *> keys, std::vector <const char *> values, void *args);
    static void gps_received_callback(const char *name, std::vector<const char *> keys, std::vector <const char *> values, void *args);

    /**
     * Member variables
     */
    jetson_camera m_flight_camera;
    image_buffer m_image_buffer;
    nrf_handler m_nrf_handler;
    target_processor m_target_detector;
    std::atomic<bool> m_is_armed;
    std::atomic<bool> m_should_sample_accel;
    accel_mma8451_pi m_lpf_accel;
    Vector3 m_last_accel;
    std::thread *m_accel_thread;

    std::atomic<bool> m_beacon_deployed;

    packet_manager::packet_callback m_arming_packet_callback;
    packet_manager::packet_callback m_flight_data_callback;
    packet_manager::packet_callback m_mode_callback;
    packet_manager::packet_callback m_gps_received_callback;

};


#endif //PROJECT_FINAL_JETSON_SYSTEM_H
