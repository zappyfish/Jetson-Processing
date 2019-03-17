//
// Created by Liam Kelly on 2/7/19.
//

#ifndef PROJECT_SUBSCALE_2_SYSTEM_H
#define PROJECT_SUBSCALE_2_SYSTEM_H

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
#include "mode_entry.h"
#include "mode_packet.h"
#include "accel_mma_8451_pi.h"
#include <atomic>
#include <chrono>
#include <thread>

class subscale2_system : public flight_system {

public:

    subscale2_system();
    ~subscale2_system();

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

    /**
     * Member variables
     */
    raspicamera m_flight_camera;
    image_buffer m_image_buffer;
    target_processor m_target_detector;
    std::atomic<bool> m_is_armed;
    std::atomic<bool> m_should_sample_accel;
    accel_mma8451_pi m_lpf_accel;
    Vector3 m_last_accel;
    std::thread m_accel_thread;

    packet_manager::packet_callback m_arming_packet_callback;
    packet_manager::packet_callback m_flight_data_callback;
    packet_manager::packet_callback m_mode_callback;
};


#endif //PROJECT_SUBSCALE_2_SYSTEM_H
