//
// Created by Liam Kelly on 10/29/18.
//

#ifndef NVIDIA_IMAGE_H
#define NVIDIA_IMAGE_H

/**
 * This may or may not be a necessary class, though we might want to associate metadata
 * with images therefore making this class useful. So long as we handle references properly,
 * we shouldn't lose efficiency.
 */

#include <cstddef>
#include <opencv2/opencv.hpp>
#include <string>


class image {

public:

    // This constructor really only exists for testing
    image(std::string file_path);

    image(cv::Mat img_mat);

    image();
    ~image();

    // Mostly have these for testing
    void set_id(int id);
    int get_id();

    std::string get_timestamp();

    void set_mat(cv::Mat &mat);

    cv::Mat get_mat_copy();
    /**
     * Please be careful with this method. It gives you a reference to the original Mat so we don't have to do the work
     * of making a deep copy. It should really only be used if you're not going to modify/process the image or you're
     * going to copy before modifying.
     * @return a reference to the original image Mat
     */
    cv::Mat& get_original_mat();

private:
    int m_id;
    std::string m_timestamp;
    cv::Mat m_image_mat;
};


#endif //NVIDIA_IMAGE_H
