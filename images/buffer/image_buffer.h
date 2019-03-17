//
// Created by Liam Kelly on 10/27/18.
//

#ifndef NVIDIA_IMAGE_BUFFER_H
#define NVIDIA_IMAGE_BUFFER_H

#define MAX_CAPACITY 3 // I think it makes sense to just have one captured and
// one on standby, but no reason not to keep generalized

#include "image.h"
#include <condition_variable>
#include <atomic>
#include <mutex>

/**
 * This MUST have a thread-safe implementation
 */

typedef std::shared_ptr<image> image_ptr;

class image_buffer {

public:

    static size_t get_max_capacity();

    image_buffer();
    ~image_buffer();

    /**
     *
     * @return true if there is space for the producer to place an image
     */
    bool has_space();

    /**
     *
     * @return true if there is an image for the consumer
     */
    bool has_image();

    /**
     *
     * @return true if successfully claimed role
     */
    bool claim_consumer();

    /**
     *
     * @return true if successfully claimed role
     */
    bool claim_producer();

    /**
     * MUST be called by a consumer after it is done with the buffer
     */
    void release_consumer();

    /**
     * MUST be called by a producer after it is done with the buffer
     */
    void release_producer();

    void place_image(image_ptr img);

    image_ptr retrieve_image();

    void empty_buffer();

private:

    static const std::chrono::milliseconds BUFFER_WAIT_TIME;

    unsigned int m_num_images;
    unsigned int m_buffer_start;
    unsigned int m_buffer_end;

    std::atomic<bool> m_has_consumer;
    std::atomic<bool> m_has_producer;

    std::condition_variable m_buffer_condition_var;
    std::mutex m_buffer_lock;

    image_ptr m_buffer[MAX_CAPACITY];
};


#endif //NVIDIA_IMAGE_BUFFER_H
