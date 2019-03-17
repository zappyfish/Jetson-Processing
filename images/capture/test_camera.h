//
// Created by Liam Kelly on 11/7/18.
//

#ifndef NVIDIA_TEST_CAMERA_H
#define NVIDIA_TEST_CAMERA_H

#include "camera.h"

/**
 * This class does weird stuff and should be used EXCLUSIVELY for testing
 */
class test_camera : public camera {

public:

    test_camera(std::string test_img_path);
    test_camera();
    ~test_camera();

    image_ptr get_test_capture_image();

    image_ptr capture_image();

private:

    static const int CAPTURE_DELAY_MS;

    image_ptr test_image;
};

#endif //NVIDIA_TEST_CAMERA_H
