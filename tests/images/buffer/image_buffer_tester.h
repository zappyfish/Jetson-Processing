//
// Created by Liam Kelly on 11/7/18.
//

#include "image_buffer.h"
#include <thread>
#include <atomic>

// Helper function
void fill_buffer_and_test_images(image_ptr *test_images, image_buffer &buffer);
void place_image(image_buffer *buffer, image_ptr img);
void retrieve_image(std::atomic<bool> *success_flag, image_buffer *buffer, image_ptr expected_img);

/**
 * This test case ensures that the image buffer can be claimed and released as expected.
 */
TEST_CASE("Claiming Image Buffer Behavior", "[image_buffer]") {
    image_buffer buffer;

    // Initial conditions
    REQUIRE(buffer.has_space());
    REQUIRE(!buffer.has_image());

    // Should be able to claim producer initially, then claiming fails until release
    REQUIRE(buffer.claim_producer());
    REQUIRE(!buffer.claim_producer());
    buffer.release_producer();
    REQUIRE(buffer.claim_producer());

    // Should be able to claim consumer initially, then claiming fails until release
    REQUIRE(buffer.claim_consumer());
    REQUIRE(!buffer.claim_consumer());
    buffer.release_consumer();
    REQUIRE(buffer.claim_consumer());
}

/**
 * This test case ensures that the image buffer can store at least as many images
 * as guaranteed by its max capacity and it will return them in the correct order
 */
TEST_CASE("Storing and Retrieving Images In Order Behavior", "[image_buffer]") {
    image_buffer buffer;

    REQUIRE(buffer.has_space());
    REQUIRE(!buffer.has_image());

    buffer.claim_producer();

    image_ptr *test_images = new image_ptr[buffer.get_max_capacity()];

    fill_buffer_and_test_images(test_images, buffer);

    REQUIRE(!buffer.has_space());
    REQUIRE(buffer.has_image());

    for (size_t i = 0; i < buffer.get_max_capacity(); i++) {
        // Reference matching
        image_ptr retrieved = buffer.retrieve_image();
        REQUIRE(retrieved.get() == test_images[i].get()); // Same image as per reference
        REQUIRE(retrieved->get_id() == test_images[i]->get_id()); // Sanity check
    }

    REQUIRE(buffer.has_space());

    // Test cleanup
    delete[] test_images;
}

/**
 * This test case ensures that the image buffer will NOT store images after its maximum
 * capacity until an image is retrieved. It then checks that they are recovered in
 * the proper order
 */

TEST_CASE("Storing And Retrieving Images at Buffer Capacity Behavior", "[image_buffer") {
    image_buffer buffer;

    buffer.claim_producer();

    image_ptr *test_images = new image_ptr[buffer.get_max_capacity() + 1];
    fill_buffer_and_test_images(test_images, buffer);

    image_ptr last_image = std::make_shared<image>();
    last_image->set_id(buffer.get_max_capacity());
    test_images[buffer.get_max_capacity()] = last_image;

    std::thread place_img_thread = std::thread(place_image, &buffer, last_image);

    for (size_t i = 0; i < buffer.get_max_capacity() + 1; i++) {
        image_ptr retrieved = buffer.retrieve_image();
        REQUIRE(retrieved.get() == test_images[i].get());
        REQUIRE(retrieved->get_id() == test_images[i]->get_id()); // Sanity check
    }

    // We verify that it got placed because it eventually joins and reaches REQUIRE(true)
    place_img_thread.join();
    REQUIRE(true);

    // Test cleanup
    delete[] test_images;
}

/**
 * This test case ensures that the image buffer won't retrieve an image until it has
 * been placed
 */

TEST_CASE("Storing and Retrieving Images When Empty Behavior", "[image_buffer]") {
    image_buffer buffer;

    buffer.claim_producer();
    image_ptr test_image = std::make_shared<image>();
    std::atomic<bool> success_flag(false);

    std::thread retrieve_img_thread = std::thread(retrieve_image, &success_flag, &buffer, test_image);

    buffer.place_image(test_image);

    retrieve_img_thread.join();

    REQUIRE(success_flag);
}

// ------------- Helper function definitions ------------ //

void place_image(image_buffer *buffer, image_ptr img) {
    buffer->place_image(img);
}

void retrieve_image(std::atomic<bool> *success_flag, image_buffer *buffer, image_ptr expected_img) {
    image_ptr retrieved = buffer->retrieve_image();
    (*success_flag) = retrieved.get() == expected_img.get() && retrieved->get_id() == expected_img->get_id();
}

void fill_buffer_and_test_images(image_ptr *test_images, image_buffer &buffer) {
    for (size_t i = 0; i < buffer.get_max_capacity(); i++) {
        image_ptr test = std::make_shared<image>();
        test->set_id(i);
        buffer.place_image(test);
        test_images[i] = test;
    }
}