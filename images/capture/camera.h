//
// Created by Liam Kelly on 10/27/18.
//

#ifndef NVIDIA_CAMERA_H
#define NVIDIA_CAMERA_H

#include "../buffer/image_buffer.h"
#include <assert.h>
#include <atomic>
#include <mutex>
#include <thread>
#include <memory>

class camera {

public:

    camera();
    ~camera();

    /**
     * This method will create a capture loop using the image_buffer passed. Note
     * that it will not start a capture loop if the buffer is already taken by
     * a camera (producer) or this camera is already in a capture loop, or if there
     * was some issue with the OS creating the thread.
     * @param buffer: an image_buffer for placing results of capture in (image objects)
     * @return true if capture successfully started, false if couldn't start
     */
    bool start_capture(image_buffer *buffer);

    void end_capture();

    bool is_capturing();

private:

    virtual image_ptr capture_image() = 0;

    void capture_loop();

    image_buffer* m_buffer;

    std::thread m_capture_loop_thread;
    // This mutex is used for initiating capture (in start_capture)
    std::mutex m_capture_mutex;
    // This mutex is used for maintaining the capture loop (and killing the thread)
    std::atomic<bool> m_should_capture;
};


#endif //NVIDIA_CAMERA_H
