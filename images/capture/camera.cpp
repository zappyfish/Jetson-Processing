//
// Created by Liam Kelly on 11/7/18.
//

#include "camera.h"
#include "soft_error_entry.h"

camera::camera() : m_should_capture(false) {}

camera::~camera() {
    end_capture();
}

bool camera::start_capture(image_buffer *buffer) {

    bool could_start_capture = false;
    // Ensure it is not already claimed & we're not already in a capture loop
    m_capture_mutex.lock();
    // NOTE: evaluation MUST happen in this order
    if(!m_should_capture && buffer->claim_producer()) {
        m_buffer = buffer;
        m_should_capture = true;
        m_capture_loop_thread = std::thread(&camera::capture_loop, this);
        could_start_capture = true;
    }
    m_capture_mutex.unlock();

    // Should probably check that thread was started successfully
    return could_start_capture;
}

void camera::end_capture() {
    if (m_should_capture) {
        m_should_capture = false;
        if (m_buffer != nullptr) {
            m_buffer->release_producer();
        } else {
            // TODO: Log soft error if buffer is null here
            std::map<std::string, std::string> metadata;
            metadata["file"] = "camera.cpp";
            metadata["method"] = "end_capture";
            soft_error_entry s_error("nullptr", "camera buffer is nullptr in end_capture", metadata);
        }
        m_capture_loop_thread.join();
    }
}

void camera::capture_loop() {
    while (m_should_capture) { // This boolean is and must be atomic
        image_ptr capture_result = capture_image();
        if (m_buffer != nullptr) {
            m_buffer->place_image(capture_result);
        } else {
            // TODO: Log soft error if buffer is null here
            std::map<std::string, std::string> metadata;
            metadata["file"] = "camera.cpp";
            metadata["method"] = "capture_loop";
            soft_error_entry s_error("nullptr", "camera buffer is nullptr in capture_loop", metadata);
        }
    }
}

bool camera::is_capturing() {
    return m_should_capture;
}