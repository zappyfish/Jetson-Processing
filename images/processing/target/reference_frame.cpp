//
// Created by Liam Kelly on 2/21/19.
//

#include "reference_frame.h"

const double reference_frame::PI = 3.14159;

reference_frame::reference_frame(double fov_degrees) {
    m_fov_radians = fov_degrees * PI / 180.0;
    // TODO: Reject f_o_v >= 90 degrees
    m_relative_height = 1 / std::tan(m_fov_radians * 0.5);
}

reference_frame::reference_frame() {}

reference_frame::~reference_frame() {}

double reference_frame::get_target_angle_from_azimuth(double target_error) {
    // error +/- 1 = +/- (1/2)f_o_v degrees
    return std::atan2(target_error, m_relative_height);
}

double reference_frame::get_earth_frame_distance(double target_error, double current_angle_degrees, double altitude) {
    return get_earth_frame_distance_from_corrected_error(get_corrected_error(target_error, current_angle_degrees), altitude);
}

double reference_frame::get_corrected_error(double target_error, double current_angle_degrees) {
    double current_angle_radians = current_angle_degrees * PI / 180.0;
    double target_angle_camera_frame = get_target_angle_from_azimuth(target_error);
    double target_angle_earth_frame =  target_angle_camera_frame + current_angle_radians;
    return m_relative_height * std::tan(target_angle_earth_frame);
}

double reference_frame::get_earth_frame_distance_from_corrected_error(double corrected_error, double altitude) {
    return altitude * corrected_error;
}

void reference_frame::get_corrected_errors(double error_x, double error_y, double roll_degrees, double pitch_degrees,
                                           double &corrected_x, double &corrected_y) {
    double pitch = -pitch_degrees * reference_frame::PI / 180.0;
    double roll = -roll_degrees * reference_frame::PI / 180.0;
    cv::Mat rot = get_x_rotation_mat(pitch) * get_y_rotation_mat(roll);
    cv::Mat error_vec = get_error_vector(error_x, error_y);
    cv::Mat result = rot * error_vec;
    corrected_x = result.at<double>(0);
    corrected_y = result.at<double>(1);
}

cv::Mat reference_frame::get_error_vector(double error_x, double error_y) {
    cv::Mat vec(3, 1, CV_64F);
    vec.at<double>(0) = error_x;
    vec.at<double>(1) = error_y;
    vec.at<double>(2) = 1;
    return vec;
}

cv::Mat reference_frame::get_x_rotation_mat(double pitch) {
    cv::Mat rot_x(3, 3, CV_64F);
    rot_x.at<double>(0, 0) = 1;
    rot_x.at<double>(0, 1) = 0;
    rot_x.at<double>(0, 2) = 0;

    rot_x.at<double>(1, 0) = 0;
    rot_x.at<double>(1, 1) = std::cos(pitch);
    rot_x.at<double>(1, 2) = -std::sin(pitch);

    rot_x.at<double>(2, 0) = 0;
    rot_x.at<double>(2, 1) = std::sin(pitch);
    rot_x.at<double>(2, 2) = std::cos(pitch);
    return rot_x;
}

cv::Mat reference_frame::get_y_rotation_mat(double roll) {
    cv::Mat rot_y(3, 3, CV_64F);
    rot_y.at<double>(0, 0) = std::cos(roll);
    rot_y.at<double>(0, 1) = 0;
    rot_y.at<double>(0, 2) = std::sin(roll);

    rot_y.at<double>(1, 0) = 0;
    rot_y.at<double>(1, 1) = 1;
    rot_y.at<double>(1, 2) = 0;

    rot_y.at<double>(2, 0) = -std::sin(roll);
    rot_y.at<double>(2, 1) = 0;
    rot_y.at<double>(2, 2) = std::cos(roll);
    return rot_y;
}