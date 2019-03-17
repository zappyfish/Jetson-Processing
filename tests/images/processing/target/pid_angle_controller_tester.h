//
// Created by Liam Kelly on 2/19/19.
//

#ifndef PROJECT_PID_ANGLE_CONTROLLER_TESTER_H
#define PROJECT_PID_ANGLE_CONTROLLER_TESTER_H

#include "pid_angle_controller.h"
#include <iostream>

TEST_CASE("Test PID Angle Controller Gives 0 for 0 error", "[pid_angle_controller]") {
    pid_angle_controller pid(1.0, 0.08, 2.2);

    double angle_x, angle_y;

    pid.get_angles_for_error(0, 0, angle_x, angle_y);

    REQUIRE(angle_x == 0);
    REQUIRE(angle_y == 0);
}

TEST_CASE("Test PID Angle Controller for decreasing error", "[pid_angle_controller]") {
    int abs_max = 20;
    pid_angle_controller pid(1.0, 0.08, 3);

    double max_x = 10000;
    double max_y = 10000;

    while (abs_max > 0) {
        double angle_x, angle_y;

        for (int i = abs_max; i > -abs_max; i--) {
            double error = (double)i / abs_max;
            pid.get_angles_for_error(error, error, angle_x, angle_y);
            // std::cout << "error: " << error << std::endl;
            // std::cout << "angle_x: " << angle_x << ", angle_y" << angle_y << std::endl;
        }

        abs_max--;

        for (int i = -abs_max; i < abs_max; i++) {
            double error = (double)i / abs_max;
            pid.get_angles_for_error(error, error, angle_x, angle_y);
            // std::cout << "error: " << error << std::endl;
            // std::cout << "angle_x: " << angle_x << ", angle_y " << angle_y << std::endl;
        }

        abs_max--;

        // REQUIRE(angle_x < max_x);
        // REQUIRE(angle_y < max_y);

        max_x = angle_x;
        max_y = angle_y;
    }
}

#endif //PROJECT_PID_ANGLE_CONTROLLER_TESTER_H
