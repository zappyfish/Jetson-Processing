//
// Created by Liam Kelly on 10/29/18.
//

#include "image.h"
#include "time_manager.h"

image::image() : m_id(-1), m_timestamp(time_manager::get_instance().get_relative_time()) {}

image::image(std::string file_path) : image() {
    m_image_mat = cv::imread(file_path);
}

image::image(cv::Mat img_mat) : image() {
    m_image_mat = img_mat;
}

image::~image() {}

int image::get_id() {
    return m_id;
}

void image::set_id(int id) {
    m_id = id;
}

std::string image::get_timestamp() {
    return m_timestamp;
}

cv::Mat image::get_mat_copy() {
    cv::Mat copy = m_image_mat.clone(); // This should be safe because it's really a typedef'd shared_ptr
    return copy;
}

cv::Mat& image::get_original_mat() {
    return m_image_mat;
}

void image::set_mat(cv::Mat &mat) {
    m_image_mat = mat;
}