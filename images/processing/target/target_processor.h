//
// Created by Liam Kelly on 10/27/18.
//

#ifndef NVIDIA_TARGET_PROCESSOR_H
#define NVIDIA_TARGET_PROCESSOR_H

#include <atomic>
#include <opencv2/opencv.hpp>
#include <thread>
#include "target_entry.h"
#include "image_buffer.h"
#include "directions_packet.h"
#include "packet_manager.h"
#include "data_logger.h"
#include "pid_angle_controller.h"
#include "reference_frame.h"
#include "flight_packet.h"


#define LIGHTNESS_THRESH 155
#define B_THRESH_MIN 150

/**
 * Abstract class
 */
class target_processor {

public:

    static const int MIN_FRAMES_BEFORE_SEARCH;

    target_processor();
    ~target_processor();

    bool is_processing();

    void send_for_processing(cv::Mat &img, std::string timestamp);

    void process_image(std::string timestamp);
    /**
     *
     * @param location: contains the location of the target in the image if it was seen
     * @returns: true if target was seen in the last image
     */
    bool get_target_location(double *location);


private:

    static const double TRIAL_KP;
    static const double TRIAL_KI;
    static const double TRIAL_KD;

    static const double LANDING_THRESHOLD;

    bool find_target(cv::Mat &img);
    void cleanup_processing_thread();
    void kmeans(cv::Mat &src);

    double m_last_location[2];
    double m_last_area_percentage;
    double m_last_altitude;
    double m_last_pitch;
    double m_last_roll;

    pid_angle_controller m_pid_controller;
    reference_frame m_frame_correction;

    bool m_saw_target;
    int m_frames_since_target;
    cv::Mat m_image_to_process;
    std::atomic<bool> m_is_processing;
    std::thread* m_processing_thread;

    bool is_square(cv::Mat&bin_img, int area, int x_center, int y_center);
    void check_square(cv::Mat&bin_img, int x_center, int y_center, int side_length, double &count);
    bool in_bounds(cv::Mat &img, int row, int col);
    bool get_should_land(double corrected_error_x, double corrected_error_y, float target_area_percentage);
    void handle_target_error_data(double error_x, double error_y, std::string timestamp);
    void set_flight_data(flight_packet &packet);

    packet_manager::packet_callback m_flight_data_callback;

    static void flight_packet_callback(const char *name, std::vector<const char *> keys, std::vector <const char *> values, void *args);
};


#endif //NVIDIA_TARGET_PROCESSOR_H
