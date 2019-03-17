//
// Created by Liam Kelly on 2/21/19.
//

#ifndef PROJECT_REFERENCE_FRAME_TESTER_H
#define PROJECT_REFERENCE_FRAME_TESTER_H

#include "reference_frame.h"
#include <iostream>

TEST_CASE("Reference Frame Test Full F.O.V Rotation", "[reference_frame]") {
    double fov = 40.0;
    double target_error = -1;

    reference_frame ref_frame(fov);

    REQUIRE(ref_frame.get_earth_frame_distance(target_error, fov, 1) == Approx(1.0));
}

TEST_CASE("Reference Frame Test Intermediate Rotations", "[reference_frame]") {
    double fov = 50.0;

    reference_frame ref_frame(fov);
    double target_error = 0;
    REQUIRE(ref_frame.get_earth_frame_distance(target_error, fov / 2, 1) == Approx(1.0));
    target_error = -1;
    REQUIRE(ref_frame.get_earth_frame_distance(target_error, fov / 2, 1) == Approx(0.0).margin(0.001));
    target_error = -0.5;
    REQUIRE(ref_frame.get_earth_frame_distance(target_error, 20, 1) == Approx(0.25859).margin(0.001));
    target_error = 0.5;
    REQUIRE(ref_frame.get_earth_frame_distance(target_error, -20, 1) == Approx(-0.25859).margin(0.001));
}

TEST_CASE("Reference Frame Test Out of Bounds Rotation", "[referencne_frame]") {
    double fov = 40.0;
    reference_frame ref_frame(fov);
    double target_error = 0.5;
    REQUIRE(ref_frame.get_earth_frame_distance(target_error, fov, 1) > 1);
    REQUIRE(ref_frame.get_earth_frame_distance(-1, fov + 1, 1) > 1);
}


TEST_CASE("Corrected Error Does Not Change For no Rotation", "[reference_frame]") {
    double fov = 40.0;
    double target_error = 0.6;
    double rotation_angle = 0;

    reference_frame ref_frame(fov);
    REQUIRE(ref_frame.get_corrected_error(target_error, rotation_angle) == Approx(target_error));
}
#endif //PROJECT_REFERENCE_FRAME_TESTER_H
