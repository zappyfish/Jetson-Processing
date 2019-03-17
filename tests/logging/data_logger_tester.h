//
// Created by Liam Kelly on 11/10/18.
//

#ifndef NVIDIA_DATA_LOGGER_TESTER_H
#define NVIDIA_DATA_LOGGER_TESTER_H

#include "data_logger.h"
#include "test_entry.h"
#include <chrono>
#include <thread>
#include "rpi_file_manager.h"
#include "log_entry.h"
#include <iostream>

const std::string DATA_LOGGER_TEST_IMAGE_PATH = "test_directory/test_data/images/test.png";
const std::string DATA_LOGGER_DIRECTORY_PATH = "test_directory/data_logger";

void dlog_fill_file_list(std::vector<std::string> &file_list, const std::string& path);
void dlog_setup();
void dlog_teardown();

/**
 * In this test case we just make sure that the data logger correctly logs an image. It does much of the same as
 * rpi_file_manager_tester
 */
TEST_CASE("Data Logger Test Images Logged", "[data_logger]") {
    dlog_setup();

    image_ptr test_image = std::make_shared<image>(DATA_LOGGER_TEST_IMAGE_PATH);

    data_logger::get_instance().log_image(test_image);

    std::string saved_image_path = DATA_LOGGER_DIRECTORY_PATH + "/vadl/flights/flight_0/images/";
    std::vector<std::string> file_list;
    dlog_fill_file_list(file_list, saved_image_path);

    REQUIRE(file_list.size() == 1);

    std::string file = file_list.at(0);
    // Hacky way to parse the filename
    size_t ind = -1;
    char c;
    std::string timestamp;

    while((c = file[++ind]) != '.') {
        timestamp += c;
        bool is_digit = c <= '9' && c >= '0';
        REQUIRE(is_digit);
    }

    ++ind; // Move past the '.'
    REQUIRE(file.substr(ind) == "jpg");

    dlog_teardown();
}

/**
 * In this test case we make sure that log entries are correctly written to test files when maximum capacity is reached
 * in the buffer
 */
TEST_CASE("Data Logger Test Log Entries Logged When Maximum Capacity is Reached", "[data_logger]") {
    dlog_setup();

    for (size_t i = 0; i < data_logger::ENTRY_BUFFER_SIZE; i++) {
        test_entry *entry = new test_entry(8, 8, 8);
        data_logger::get_instance().save_log_entry(entry);
    }

    /**
     * What proceeds is INCREDIBLY hacky and non-deterministic, but I'm not sure how to do it in another way.
     * Like not gonna lie I'm actually just...sad about this :'(
     */
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::string path = DATA_LOGGER_DIRECTORY_PATH + "/vadl/flights/flight_0/TEST_ENTRY.txt";

    REQUIRE(exists(path));

    dlog_teardown();
}

TEST_CASE("Data Logger Test Creates Different Files For Different Entry Types", "[data_logger]") {
    dlog_setup();

    test_entry *entry1 = new test_entry(8, 8, 8);
    data_logger::get_instance().save_log_entry(entry1);

    std::map<std::string, std::string> s_error_metadata;
    std::string description = "description";
    std::string category = "category";

    soft_error_entry *entry2 = new soft_error_entry(category, description, s_error_metadata);
    data_logger::get_instance().save_log_entry(entry2);

    data_logger::get_instance().write_remaining_entries();

    std::string path1 = DATA_LOGGER_DIRECTORY_PATH + "/vadl/flights/flight_0/TEST_ENTRY.txt";
    std::string path2 = DATA_LOGGER_DIRECTORY_PATH + "/vadl/flights/flight_0/SOFT_ERROR.txt";

    REQUIRE(exists(path1));
    REQUIRE(exists(path2));

    dlog_teardown();
}

/**
 * In this test case we make sure that log entries are correctly written when we call write_remaining_entries
 */
TEST_CASE("Data Logger Test Log Remaining Entries", "[data_logger]") {
    dlog_setup();

    test_entry *entry = new test_entry(8, 8, 8);

    data_logger::get_instance().save_log_entry(entry);
    data_logger::get_instance().write_remaining_entries();

    std::string path = DATA_LOGGER_DIRECTORY_PATH + "/vadl/flights/flight_0/TEST_ENTRY.txt";


    REQUIRE(exists(path));

    dlog_teardown();
}

TEST_CASE("Data Logger Creates Different Flight Sessions and Terminates them Properly", "[data_logger]") {
    remove_all("test_directory/data_logger"); // Just make sure we're in a good environment

    if (!exists("test_directory")) {
        create_directory("test_directory");
    }
    if (!exists("test_directory/data_logger")) {
        create_directory("test_directory/data_logger");
    }

    rpi_file_manager *f_manager1 = new rpi_file_manager(DATA_LOGGER_DIRECTORY_PATH);
    rpi_file_manager *f_manager2 = new rpi_file_manager(DATA_LOGGER_DIRECTORY_PATH);

    data_logger::get_instance().start_flight_session(f_manager1);
    data_logger::get_instance().start_flight_session(f_manager2);

    std::string path1 = DATA_LOGGER_DIRECTORY_PATH + "/vadl/flights/flight_0";
    std::string path2 = DATA_LOGGER_DIRECTORY_PATH + "/vadl/flights/flight_1";

    REQUIRE(exists(path1));
    REQUIRE(exists(path2));

    dlog_teardown();
}

TEST_CASE("Data Logger Test Overload With Logging", "[data_logger]") {
    for (int i = 0; i < 10000; i++) {
        test_entry *entry = new test_entry(i, i, i);
        data_logger::get_instance().save_log_entry(entry);
    }
}

// ----------------------- Helper functions ------------------- //

void dlog_fill_file_list(std::vector<std::string> &file_list, const std::string& path) {
    if (!path.empty()) {
        namespace fs = boost::filesystem;

        fs::path apk_path(path);
        fs::recursive_directory_iterator end;

        for (fs::recursive_directory_iterator i(apk_path); i != end; ++i) {
            const fs::path cp = (*i);
            file_list.push_back(cp.filename().string());
        }
    }
}

void dlog_setup() {
    // dlog_teardown();
    remove_all("test_directory/data_logger");
    if (!exists("test_directory")) {
        create_directory("test_directory");
    }
    create_directory("test_directory/data_logger");
    // Should probably create a test file manager, but we'll use rpi for now
    rpi_file_manager *f_manager = new rpi_file_manager(DATA_LOGGER_DIRECTORY_PATH);
    data_logger::get_instance().start_flight_session(f_manager);
}

void dlog_teardown() {
    data_logger::get_instance().end_flight_session();
    remove_all("test_directory/data_logger");
}

#endif //NVIDIA_DATA_LOGGER_TESTER_H
