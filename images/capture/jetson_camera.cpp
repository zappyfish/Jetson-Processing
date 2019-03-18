//
// Created by Liam Kelly on 1/26/19.
//

#include "jetson_camera.h"

#ifdef __linux__

#include "soft_error_entry.h"
#include "data_logger.h"

const int jetson_camera::DEFAULT_HEIGHT = 1000;
const int jetson_camera::DEFAULT_WIDTH = 1000;
const int jetson_camera::DEFAULT_FPS = 60;

jetson_camera::jetson_camera(int width, int height, int fps) : m_capture_camera(get_tegra_pipeline(width, height, fps), cv::CAP_GSTREAMER) {
    if (!m_capture_camera.isOpened()) {
        std::string category = "camera";
        std::string description = "could not open camera";
        std::map<std::string, std::string> metadata;
        metadata["file"] = "jetson_camera.cpp";
        metadata["method"] = "constructor";
        soft_error_entry *s_error = new soft_error_entry(category, description, metadata);
        data_logger::get_instance().save_log_entry(s_error);
    }
}

jetson_camera::~jetson_camera() {}

image_ptr jetson_camera::capture_image() {
    image_ptr img = std::make_shared<image>();
    cv::Mat result;
    m_capture_camera >> result;
    img->set_mat(result);
    return img;
}

#endif