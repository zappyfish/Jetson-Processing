//
// Created by Liam Kelly on 11/8/18.
//

#ifndef NVIDIA_ARDUCAM_TESTER_H
#define NVIDIA_ARDUCAM_TESTER_H

#include "arducam.h"
#include <iostream>

void arducam_retrieve_image_fcn(image_buffer *buffer, image_ptr *retrieved_img);

/**
 * For now, this is mainly testing smart pointer behavior.
 */
TEST_CASE("Arducam Returns Usable Data", "[arducam]") {
    arducam camera;
    image_buffer *buffer = new image_buffer();

    camera.start_capture(buffer);

    image_ptr retrieved_img = std::make_shared<image>();

    std::thread retrieval_thread(arducam_retrieve_image_fcn, buffer, &retrieved_img);
    //std::thread retrieval_thread(test_thread);
    // Wait for retrieval to occur
    retrieval_thread.join();

    REQUIRE(retrieved_img.get() != nullptr);
}

TEST_CASE("Nick Was Standing In Front of Camera", "[arducam]") {
    REQUIRE(true);
}

// --------------------- Helper functions --------------------- //

void arducam_retrieve_image_fcn(image_buffer *buffer, image_ptr *retrieved_img) {
    (*retrieved_img) = buffer->retrieve_image();
}

#endif //NVIDIA_ARDUCAM_TESTER_H
