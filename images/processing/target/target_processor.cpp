//
// Created by Liam Kelly on 10/27/18.
//

#include "target_processor.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <math.h>

using namespace std;
using namespace cv;

const int target_processor::MIN_FRAMES_BEFORE_SEARCH = 30; // Play around with me
const double target_processor::TRIAL_KP = 1.5;
const double target_processor::TRIAL_KI = 0;
const double target_processor::TRIAL_KD = 4;

const double target_processor::LANDING_THRESHOLD = 0.5;

target_processor::target_processor() : m_is_processing(false), m_saw_target(false), m_processing_thread(nullptr),
m_pid_controller(TRIAL_KP, TRIAL_KI, TRIAL_KD),
m_last_altitude(0), m_last_pitch(0), m_last_roll(0) {

    m_last_location[0] = 0;
    m_last_location[1] = 0;

    m_flight_data_callback.name = flight_packet::PACKET_NAME;
    m_flight_data_callback.args = this;
    m_flight_data_callback.callback = &target_processor::flight_packet_callback;

    packet_manager::get_instance().set_packet_callback(&m_flight_data_callback);
}

target_processor::~target_processor() {
    cleanup_processing_thread();
    packet_manager::get_instance().remove_packet_callback(&m_flight_data_callback);
}

void target_processor::cleanup_processing_thread() {
    if (m_processing_thread != nullptr) {
        m_processing_thread->join();
        delete m_processing_thread;
    }
}

bool target_processor::is_processing() {
    return m_is_processing.load();
}

bool target_processor::get_target_location(double *location) {
    if (m_saw_target) {
        location[0] = m_last_location[0];
        location[1] = m_last_location[1];
    }
    return m_saw_target;
}

void target_processor::send_for_processing(cv::Mat &img, std::string timestamp) {
    if (!m_is_processing.load()) {
        cleanup_processing_thread();
        if (img.total() != 0 && img.data != nullptr) {
            m_image_to_process = img;
            m_processing_thread = new std::thread(&target_processor::process_image, this, timestamp);
        }
    }
}

void target_processor::process_image(std::string timestamp) {
    m_is_processing = true;
    m_saw_target = find_target(m_image_to_process);

    if (m_saw_target) {
        double error_x = m_last_location[0] / (m_image_to_process.cols / 2); // uncorrected camera frame errors
        double error_y = m_last_location[1] / (m_image_to_process.rows / 2);
        handle_target_error_data(error_x, error_y, timestamp);
    }
//    if (m_saw_target) {
//        m_frames_since_target = 0;
//        double error_x = m_last_location[0] / (m_image_to_process.cols / 2); // uncorrected camera frame errors
//        double error_y = m_last_location[1] / (m_image_to_process.rows / 2);
//        handle_target_error_data(error_x, error_y, timestamp);
//    } else {
//        if (m_frames_since_target >= MIN_FRAMES_BEFORE_SEARCH) {
//            directions_packet* begin_search_directions = new directions_packet(0, 0, false, false);
//            packet_manager::get_instance().send_packet(begin_search_directions);
//        } else {
//            m_frames_since_target++;
//        }
//    }
    m_is_processing = false;
}

void target_processor::handle_target_error_data(double error_x, double error_y, std::string timestamp) {

    double corrected_error_x, corrected_error_y;

    m_frame_correction.get_corrected_errors(error_x, error_y, m_last_roll, m_last_pitch, corrected_error_x, corrected_error_y);

    double send_angle_x, send_angle_y;

    m_pid_controller.get_angles_for_error(corrected_error_x, corrected_error_y, send_angle_x, send_angle_y);

    bool should_land = get_should_land(corrected_error_x, corrected_error_y, m_last_area_percentage);

    target_entry* found_target_log = new target_entry(timestamp, m_last_location[0], m_last_location[1],
            corrected_error_x, corrected_error_y, send_angle_x, send_angle_y, should_land);
    data_logger::get_instance().save_log_entry(found_target_log);

    // TODO: Send a directions packet

     directions_packet* target_found_directions = new directions_packet(send_angle_x, send_angle_y, true, should_land);
     packet_manager::get_instance().send_packet(target_found_directions);
}

bool target_processor::find_target(cv::Mat &img) {

    Mat gray_img;

    cv::cvtColor(img, gray_img, cv::COLOR_RGB2GRAY);

    cv::Mat ret, blur;

    cv::GaussianBlur(gray_img, blur, Size(5,5), 0);
    cv::threshold(blur, ret, 0, 255, THRESH_BINARY+THRESH_OTSU);


    cv::Mat structure_element1 = cv::getStructuringElement(MORPH_RECT,Size(20,20));
    cv::Mat structure_element2 = cv::getStructuringElement(MORPH_RECT,Size(10,10));
    cv::dilate(ret, ret, structure_element2);
    cv::erode(ret, ret, structure_element1);

    std::vector<std::vector<cv::Point> >contours;
    std::vector<cv::Vec4i>hierarchy;
    int savedContour = -1;
    double maxArea = 0.0;
    cv::findContours(ret, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);

    for (int i = 0; i< contours.size(); i++)
    {
        double area = contourArea(contours[i]);
        if (area > maxArea)
        {
            maxArea = area;
            savedContour = i;
        }
    }

    double epsilon = 0.1*arcLength(contours[savedContour], true);
    cv::Mat approx;
    cv::approxPolyDP(contours[savedContour],approx,epsilon,true);
    if (approx.rows == 4 && contours.size() < 5) {
        cv::polylines(ret, approx, true, Scalar(0,0,255), 2);
        cv::Moments m = cv::moments(contours[savedContour]);
        cv::Point2f cpt = cv::Point2f( m.m10/m.m00 , m.m01/m.m00 );
        double r_center = img.rows/2;
        double c_center = img.cols/2;
        m_last_location[0] = r_center - cpt.x;
        m_last_location[1] = cpt.y - c_center;
        return true;
    } else {
        return false;
    }

}

bool target_processor::is_square(cv::Mat&bin_img, int area, int x_center, int y_center) {
    int side_length = (int)sqrt(area);
    float thresh = 0.8; // TODO: mess around with me
    double intersection_count = 0.0;
    // Check in a cross kind of intersection:
    // 0 1 0
    // 1 1 1
    // 0 1 0

    check_square(bin_img, x_center, y_center, side_length, intersection_count);

    check_square(bin_img, x_center - side_length, y_center, side_length, intersection_count);
    check_square(bin_img, x_center + side_length, y_center, side_length, intersection_count);
    check_square(bin_img, x_center, y_center - side_length, side_length, intersection_count);
    check_square(bin_img, x_center, y_center + side_length, side_length, intersection_count);

    return (intersection_count / area >= thresh);
}

//void target_processor::kmeans(cv::Mat &src) {
//
//    blur(src,src,Size(15,15));
//
//    Mat p = Mat::zeros(src.cols*src.rows, 5, CV_32F);
//    Mat bestLabels, centers, clustered, Lab;
//    vector<Mat> bgr;
//    vector<Mat> L_a_b;
//    cvtColor(src, Lab, COLOR_BGR2Lab);
//    //cv::split(src, bgr);
//    split(Lab,L_a_b);
//    for(int i=0; i<src.cols*src.rows; i++) {
//        p.at<float>(i,0) = (i/src.cols) / src.rows;
//        p.at<float>(i,1) = (i%src.cols) / src.cols;
//        p.at<float>(i,2) = L_a_b[0].data[i] / 255.0;
//        p.at<float>(i,3) = L_a_b[1].data[i] / 255.0;
//        p.at<float>(i,4) = L_a_b[2].data[i] / 255.0;
//    }
//
//    int K = 2;
//    cv::kmeans(p, K, bestLabels,
//               TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 10, 1.0),
//               3, KMEANS_PP_CENTERS, centers);
//
//    int colors[K];
//    for(int i=0; i<K; i++) {
//        colors[i] = 255/(i+1);
//    }
//
//    clustered = Mat(src.rows, src.cols, CV_32F);
//    for(int i=0; i<src.cols*src.rows; i++) {
//        clustered.at<float>(i/src.cols, i%src.cols) = (float)(colors[bestLabels.at<int>(0,i)]);
//    }
//
//    clustered.convertTo(clustered, CV_8U);
//
//}
//
void target_processor::check_square(cv::Mat&bin_img, int x_center, int y_center, int side_length, double &count) {
    int half_side = side_length / 2;
    for (int i = x_center - half_side; i < x_center + half_side; i++) {
        for (int j = y_center - half_side; j < y_center + half_side; j++) {
            if (in_bounds(bin_img, j, i) && bin_img.at<uchar>(j, i) == 1) {
                count++;
            }
        }
    }
}

void target_processor::set_flight_data(flight_packet &packet) {
    m_last_roll = packet.get_roll();
    m_last_pitch = packet.get_pitch();
    double altitude = 0;
    if (packet.get_z() > 0) {
        altitude = packet.get_z();
    }
    m_last_altitude = altitude;
}

bool target_processor::in_bounds(cv::Mat &img, int row, int col) {
    return row >= 0 && row < img.rows && col >= 0 && col < img.cols;
}

bool target_processor::get_should_land(double corrected_error_x, double corrected_error_y, float target_area_percentage) {
    // TODO: complete me
    return corrected_error_x < LANDING_THRESHOLD && corrected_error_x > -LANDING_THRESHOLD &&
    corrected_error_y < LANDING_THRESHOLD && corrected_error_y > -LANDING_THRESHOLD;
}

void target_processor::flight_packet_callback(const char *name, std::vector<const char *> keys, std::vector <const char *> values, void *args) {
    target_processor *processor = static_cast<target_processor*>(args);
    flight_packet packet(keys, values);
    processor->set_flight_data(packet);
}
