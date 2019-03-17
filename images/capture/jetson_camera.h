//
// Created by Liam Kelly on 1/26/19.
//

#ifndef PROJECT_JETSON_CAMERA_H
#define PROJECT_JETSON_CAMERA_H

#include "camera.h"

class jetson_camera : public camera {

public:

    static const int DEFAULT_WIDTH;
    static const int DEFAULT_HEIGHT;
    static const int DEFAULT_FPS;

    jetson_camera(int width, int height, int fps);
    ~jetson_camera();

    image_ptr capture_image();

private:

    static std::string get_tegra_pipeline(int width, int height, int fps) {
        return "nvcamerasrc ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(width) + ", height=(int)" +
               std::to_string(height) + ", format=(string)I420, framerate=(fraction)" + std::to_string(fps) +
               "/1 ! nvvidconv flip-method=2 ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
    }

    cv::VideoCapture m_capture_camera;
};


#endif //PROJECT_JETSON_CAMERA_H
