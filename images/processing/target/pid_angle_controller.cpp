//
// Created by Liam Kelly on 2/19/19.
//

#include "pid_angle_controller.h"

const int pid_angle_controller::ANGLE_MULTIPLIER = 500;

pid_angle_controller::pid_angle_controller(double k_p, double k_i, double k_d) : m_k_p(k_p), m_k_i(k_i), m_k_d(k_d),
m_last_error_y(0), m_last_error_x(0), m_error_integral_y(0), m_error_integral_x(0) {}

pid_angle_controller::~pid_angle_controller() {}

void pid_angle_controller::get_angles_for_error(double error_x, double error_y, double &angle_x, double &angle_y) {
    m_error_integral_x += error_x * m_k_i;
    m_error_integral_y += error_y * m_k_i;


    double deriv_x = m_k_d * (error_x - m_last_error_x);
    double deriv_y = m_k_d * (error_y - m_last_error_y);

    m_last_error_x = error_x;
    m_last_error_y = error_y;

    double proportional_x = error_x * m_k_p;
    double proportional_y = error_y * m_k_p;

    angle_x = ANGLE_MULTIPLIER * (deriv_x + m_error_integral_x + proportional_x);
    angle_y = -ANGLE_MULTIPLIER * (deriv_y + m_error_integral_y + proportional_y);
}