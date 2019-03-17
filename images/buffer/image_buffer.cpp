//
// Created by Liam Kelly on 10/27/18.
//

#include "image_buffer.h"
#include <chrono>
#include <iostream>
#include "time_manager.h"

const std::chrono::milliseconds image_buffer::BUFFER_WAIT_TIME = std::chrono::milliseconds(200);

size_t image_buffer::get_max_capacity() {
    return MAX_CAPACITY;
}

image_buffer::image_buffer(): m_num_images(0), m_buffer_start(0), m_buffer_end(0),
m_has_consumer(false), m_has_producer(false) {}

image_buffer::~image_buffer() {
    // TODO: log soft error here
}

bool image_buffer::has_space() {
    return m_num_images < MAX_CAPACITY;
}

bool image_buffer::has_image() {
    return m_num_images > 0;
}

bool image_buffer::claim_consumer() {
    if (m_has_consumer) {
        return false;
    } else {
        m_has_consumer = true;
        return true;
    }
}

bool image_buffer::claim_producer() {
    if (m_has_producer) {
        return false;
    } else {
        m_has_producer = true;
        return true;
    }
}

void image_buffer::release_consumer() {
    m_has_consumer = false;
}

void image_buffer::release_producer() {
    m_has_producer = false;
}

void image_buffer::place_image(image_ptr img) {
    // Lock the space check
    std::unique_lock<std::mutex> lock(m_buffer_lock);
    while (!has_space()) {
        m_buffer_condition_var.wait_for(lock, BUFFER_WAIT_TIME);
        // The producer released it, trying to end capture, possibly in destruction. Therefore we want to
        // let it escape.
        if (!m_has_producer) {
            return;
        }
    }
    // Place img in buffer and update count and buffer_end
    m_buffer[m_buffer_end++] = img;
    if (m_buffer_end >= MAX_CAPACITY) {
        m_buffer_end = 0;
    }
    m_num_images++;
    lock.unlock();
    m_buffer_condition_var.notify_all();
}

/**
 *
 * @return nullptr if no producer (i.e. was released while waiting for an image)
 */
image_ptr image_buffer::retrieve_image() {
    // Lock the has_resources check
    std::unique_lock<std::mutex> lock(m_buffer_lock);
    while (!has_image()) {
        m_buffer_condition_var.wait_for(lock, BUFFER_WAIT_TIME);

        // The producer released it, trying to end capture, possibly in destruction. Therefore we want to let the
        // consumer escape.
        if (!m_has_producer) {
            image_ptr dud; // Flight is likely over, so it doesn't matter that we're getting a dud image.
            return dud;
        }
    }
    // Grab img from buffer and update count and buffer_start
    image_ptr img = m_buffer[m_buffer_start++];
    if (m_buffer_start >= MAX_CAPACITY) {
        m_buffer_start = 0;
    }
    m_num_images--;
    lock.unlock();
    m_buffer_condition_var.notify_all();
    return img;
}

void image_buffer::empty_buffer() {
    std::unique_lock<std::mutex> lock(m_buffer_lock);
    m_num_images = m_buffer_end = m_buffer_start = 0;
    for (size_t i = 0; i < MAX_CAPACITY; i++) {
        m_buffer[i].reset(); // Clear up memory
    }
    lock.unlock();
    m_buffer_condition_var.notify_all();
}