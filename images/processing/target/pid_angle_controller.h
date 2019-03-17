//
// Created by Liam Kelly on 2/19/19.
//

#ifndef PROJECT_PID_ANGLE_CONTROLLER_H
#define PROJECT_PID_ANGLE_CONTROLLER_H


class pid_angle_controller {

public:

    pid_angle_controller(double k_p, double k_i, double k_d);
    ~pid_angle_controller();

    void get_angles_for_error(double error_x, double error_y, double &angle_x, double &angle_y);

private:

    static const int ANGLE_MULTIPLIER;

    double m_last_error_x;
    double m_error_integral_x;
    double m_last_error_y;
    double m_error_integral_y;

    double m_k_p;
    double m_k_i;
    double m_k_d;
};


#endif //PROJECT_PID_CONTROLLER_H
