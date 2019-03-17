//
// Created by Liam Kelly on 11/7/18.
//

#ifndef NVIDIA_CAMERA_TESTER_H
#define NVIDIA_CAMERA_TESTER_H

#include "test_camera.h"
#include "image_buffer.h"
#include <iostream>

void retrieve_image_fcn(image_buffer *buffer, image_ptr *retrieved_img);

TEST_CASE("Test Camera Capture Loop Start and End Behavior", "[test_camera]") {
    test_camera camera;

    REQUIRE(!camera.is_capturing());

    image_buffer *buffer1 = new image_buffer();
    image_buffer *buffer2 = new image_buffer();

    // Can only capture with one buffer at a time
    REQUIRE(camera.start_capture(buffer1));
    REQUIRE(!camera.start_capture(buffer2));

    // Should be capturing now
    REQUIRE(camera.is_capturing());

    camera.end_capture();

    // Now we can capture with second buffer after ending
    REQUIRE(camera.start_capture(buffer2));

    delete buffer1;
    delete buffer2;
}

/**
 * Verify that the camera captures images and that they go in the buffer
 */
TEST_CASE("Test Camera Places Images in Buffer", "[test_camera]") {
    test_camera camera;
    image_buffer *buffer = new image_buffer();

    camera.start_capture(buffer);

    image_ptr retrieved_img = std::make_shared<image>();

    std::thread retrieval_thread(retrieve_image_fcn, buffer, &retrieved_img);
    //std::thread retrieval_thread(test_thread);
    // Wait for retrieval to occur
    retrieval_thread.join();
    // This verifies that an image was captured (because we'll block on retrieve image
    // thread until one is available AND it verifies it was the correct image)
    REQUIRE(camera.get_test_capture_image().get() == retrieved_img.get());

    camera.end_capture();
    // Clean up buffer here
    delete buffer;
}

void retrieve_image_fcn(image_buffer *buffer, image_ptr *retrieved_img) {
    (*retrieved_img) = buffer->retrieve_image();
}

#endif //NVIDIA_CAMERA_TESTER_H
