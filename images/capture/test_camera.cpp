//
// Created by Liam Kelly on 11/7/18.
//

#include "test_camera.h"
#include <chrono>
#include <thread>

const int test_camera::CAPTURE_DELAY_MS = 35;

test_camera::test_camera(std::string test_img_path) : camera(), test_image(new image(test_img_path)) {}

test_camera::test_camera() : camera(), test_image(new image) {}

test_camera::~test_camera() {}

image_ptr test_camera::get_test_capture_image() {
    return test_image;
}

image_ptr test_camera::capture_image() {
    std::this_thread::sleep_for(std::chrono::milliseconds(test_camera::CAPTURE_DELAY_MS));
    return get_test_capture_image();
}

