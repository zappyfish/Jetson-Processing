//
// Created by Liam Kelly on 11/10/18.
//

#ifndef NVIDIA_IMAGE_TESTER_H
#define NVIDIA_IMAGE_TESTER_H

#endif //NVIDIA_IMAGE_TESTER_H

#include "image.h"
#include <chrono>
#include <thread>

const std::string TEST_IMAGE_PATH = "test_directory/image/test.png";

/**
 * In this method, we make sure that get_original_mat returns the same reference to the original Mat which stores the
 * image.
 */
TEST_CASE("Image Test Get Original Mat Produces The Same Reference", "[image]") {
    image img(TEST_IMAGE_PATH);

    // Just timing the code here
    auto start_time = std::chrono::high_resolution_clock::now();
    cv::Mat mat1 = img.get_original_mat();
    cv::Mat mat2 = img.get_original_mat();
    auto end_time = std::chrono::high_resolution_clock::now();
    auto elapsed = end_time - start_time;

    // std::cout << "Getting Original Mat Twice Took: " << std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() << " microseconds\n";

    REQUIRE(mat1.data == mat2.data);
}

/**
 * In this test case, we make sure that getting a copy of the image produces a truly deep copy. We make sure that
 * references are different, and we make sure that editing images from get_mat_copy won't affect other images obtained
 * from that method.
 */
TEST_CASE("Image Test Get Mat Copy Gives Different References", "[image]") {
    image img(TEST_IMAGE_PATH);

    cv::Mat orig_mat = img.get_original_mat();

    // Just timing the code here
    auto start_time = std::chrono::high_resolution_clock::now();
    cv::Mat mat1 = img.get_mat_copy();
    cv::Mat mat2 = img.get_mat_copy();
    auto end_time = std::chrono::high_resolution_clock::now();
    auto elapsed = end_time - start_time;

    // std::cout << "Copying Mat Twice Took: " << std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() << " microseconds\n";

    REQUIRE(orig_mat.data != mat1.data);
    REQUIRE(orig_mat.data != mat2.data);
    REQUIRE(mat1.data != mat2.data);

    // Now edit a pixel and make sure other are unchanged.

    mat1.at<unsigned char>(0, 0, 0) = 255 - mat1.at<unsigned char>(0, 0, 0);
    REQUIRE(orig_mat.at<unsigned char>(0, 0, 0) != mat1.at<unsigned char>(0, 0, 0));
    REQUIRE(orig_mat.at<unsigned char>(0, 0, 0) == mat2.at<unsigned char>(0, 0, 0));
}

/**
 * In this test case, we make sure that timestamps are sequentially assigned and are roughly correct.
 */
TEST_CASE("Image Test Timestamps Are Assigned Correctly", "[image]") {
    image img1(TEST_IMAGE_PATH);

    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    image img2(TEST_IMAGE_PATH);

    int difference = std::stoi(img2.get_timestamp(), nullptr) - std::stoi(img1.get_timestamp(), nullptr);
    bool condition = (difference >= 20); // Has to be at least 20...otherwise, we can't really say. Ton of variance due
    // to what processor the test case is running on

    // std::cout << "expected diff is 20, actual is " << difference << std::endl;

    REQUIRE(condition);
}

/**
 * In this test case, we make sure that the constructor which takes a cv::Mat correctly assigns the mat and it can be
 * gotten back
 */
TEST_CASE("Image Test Mat Constructor Correctly Assigns Member Variable Mat", "[image]") {
    cv::Mat test_img = cv::imread(TEST_IMAGE_PATH);

    image img(test_img);

    REQUIRE(img.get_original_mat().data == test_img.data);
}