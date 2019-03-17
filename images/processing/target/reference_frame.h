//
// Created by Liam Kelly on 2/21/19.
//

#ifndef PROJECT_REFERENCE_FRAME_H
#define PROJECT_REFERENCE_FRAME_H

#include <cmath>
#include <opencv2/opencv.hpp>


class reference_frame {

public:

    reference_frame(double fov_degrees);
    reference_frame();
    ~reference_frame();

    /**
     *
     * @param target_error: distance from center of image, 0 to 1
     * @param current_angle: current angle in this axis of the camera relative to the "azimuth" (in this axis) in degrees
     * tilt up is positive, tilt down is negative
     * @param altitude: the current altitude of the camera
     * @return the actual distance in this axis
     */
    double get_earth_frame_distance(double target_error, double current_angle_degrees, double altitude);
    double get_corrected_error(double target_error, double current_angle_degrees);
    double get_earth_frame_distance_from_corrected_error(double corrected_error, double altitude);

    void get_corrected_errors(double error_x, double error_y, double roll_degrees, double pitch_degrees, double &corrected_x, double &corrected_y);

private:

    static const double PI;

    double m_fov_radians;
    double m_relative_height; // This is the tan of 1/2 the f.o.v (it treats the width of the plane as 2 b/c error ~ [-1, 1]

    double get_target_angle_from_azimuth(double target_error);

    cv::Mat get_x_rotation_mat(double pitch);
    cv::Mat get_y_rotation_mat(double roll);
    cv::Mat get_error_vector(double error_x, double error_y);
};


#endif //PROJECT_REFERENCE_FRAME_H
