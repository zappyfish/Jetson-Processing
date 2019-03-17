//
// Created by Liam Kelly on 11/8/18.
//

#ifndef NVIDIA_RPI_FILE_MANAGER_TESTER_H
#define NVIDIA_RPI_FILE_MANAGER_TESTER_H

#include "rpi_file_manager.h"
#include <boost/filesystem.hpp>
#include <ctime>
#include "time_manager.h"
#include <iostream>

using namespace boost::filesystem;
using namespace boost::lambda;
namespace io = boost::iostreams;

void rpi_fill_file_list(std::vector<std::string> &file_list, const std::string& path);
void rpi_setup();
void rpi_teardown();

const std::string RPI_DIRECTORY_PATH = "test_directory/rpi_file_manager";
const std::string RPI_TEST_IMAGE_PATH = "test_directory/image/test.png";


/**
 * This test case makes sure that f_manager will write lines to files, will not overwrite, and saves properly.
 * It specifically ensures it can create a new file if it doesn't exist, and it will save all writes.
 */
TEST_CASE("RPI File Manager Test Writing Lines", "[rpi_file_manager]") {
    rpi_setup();

    rpi_file_manager f_manager(RPI_DIRECTORY_PATH);

    f_manager.write_line_to_file("test.txt", "hello_world_from_liam\n");
    f_manager.write_line_to_file("test.txt", "hello_world_from_emily\n");

    ifstream file(RPI_DIRECTORY_PATH + "/vadl/flights/flight_0/test.txt");
    std::string line;

    getline(file, line);
    REQUIRE(line == "hello_world_from_liam");
    getline(file, line);
    REQUIRE(line == "hello_world_from_emily");

    // Here we ensure that there is nothing else in the file
    REQUIRE(!getline(file, line));

    rpi_teardown();
}

TEST_CASE("RPI File Manager Test Creates Flight Info File", "[rpi_file_manager]") {
    rpi_setup();

    long cur_time = std::time(0);
    rpi_file_manager f_manager(RPI_DIRECTORY_PATH);

    ifstream file(RPI_DIRECTORY_PATH + "/vadl/flights/flight_0/flight_info.txt");
    std::string line;

    getline(file, line);

    // This is a hacky way to extract the seconds since epoch
    char c;
    size_t ind = -1;
    std::string seconds_since_epoch;
    bool saw_colon = false;

    while ((c = line[++ind]) != '\n') {
        if (saw_colon) {
            seconds_since_epoch += line[ind];
        }
        saw_colon = saw_colon || (line[ind] == ':');
    }

    // We know that the file_manager was created after the time held by cur_time, so it should be >=
    REQUIRE(cur_time <= std::stol(seconds_since_epoch));

    rpi_teardown();
}

TEST_CASE("RPI File Manager Makes New Directory For Each Flight", "[rpi_file_manager]") {
    rpi_setup();

    rpi_file_manager f_manager1(RPI_DIRECTORY_PATH);
    REQUIRE(exists(RPI_DIRECTORY_PATH + "/vadl/flights/flight_0"));
    REQUIRE(!exists(RPI_DIRECTORY_PATH + "/vadl/flights/flight_1"));

    rpi_file_manager f_manager2(RPI_DIRECTORY_PATH);
    REQUIRE(exists(RPI_DIRECTORY_PATH + "/vadl/flights/flight_0"));
    REQUIRE(exists(RPI_DIRECTORY_PATH + "/vadl/flights/flight_1"));

    rpi_teardown();
}

TEST_CASE("RPI File Manager Test Multiline Write Array ", "[rpi_file_manager]") {
    rpi_setup();

    rpi_file_manager f_manager(RPI_DIRECTORY_PATH);
    std::string test_lines[] = {"hello_world_from_liam\n", "hello_world_from_emily\n"};

    f_manager.multiline_write("test.txt", test_lines, 2);

    ifstream file(RPI_DIRECTORY_PATH + "/vadl/flights/flight_0/test.txt");
    std::string line;

    getline(file, line);
    REQUIRE(line == "hello_world_from_liam");
    getline(file, line);
    REQUIRE(line == "hello_world_from_emily");

    // Here we ensure that there is nothing else in the file
    REQUIRE(!getline(file, line));

    rpi_teardown();
}

TEST_CASE("RPI File Manager Test Multiline Write Vector", "[rpi_file_manager]") {
    rpi_setup();

    rpi_file_manager f_manager(RPI_DIRECTORY_PATH);
    std::vector<std::string> lines;
    lines.push_back("hello_world_from_liam\n");
    lines.push_back("hello_world_from_emily\n");

    f_manager.multiline_write("test.txt", lines);

    ifstream file(RPI_DIRECTORY_PATH + "/vadl/flights/flight_0/test.txt");
    std::string line;

    getline(file, line);
    REQUIRE(line == "hello_world_from_liam");
    getline(file, line);
    REQUIRE(line == "hello_world_from_emily");

    // Here we ensure that there is nothing else in the file
    REQUIRE(!getline(file, line));

    rpi_teardown();
}

TEST_CASE("RPI File Manager Test Image Saving", "[rpi_file_manager]") {
    rpi_setup();

    time_manager::get_instance().reset_clock();
    rpi_file_manager f_manager(RPI_DIRECTORY_PATH);
    image_ptr test_image = std::make_shared<image>(RPI_TEST_IMAGE_PATH);

    f_manager.write_image(test_image);

    std::string saved_image_path = RPI_DIRECTORY_PATH + "/vadl/flights/flight_0/images/";
    std::vector<std::string> file_list;
    rpi_fill_file_list(file_list, saved_image_path);

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

    rpi_teardown();
}

TEST_CASE("RPI File Manager Default Constructor Creates Directory", "[rpi_file_manager]") {
    rpi_file_manager f_manager;

    REQUIRE(exists("./vadl/flights/flight_0"));

    remove_all("./vadl/flights");
}

// ----------------- Helper functions ---------------- //

void rpi_fill_file_list(std::vector<std::string> &file_list, const std::string& path) {
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

void rpi_setup() {
    rpi_teardown();
    if (!exists("test_directory")) {
        create_directory("test_directory");
    }
    create_directory("test_directory/rpi_file_manager");
}

void rpi_teardown() {
    remove_all("test_directory/rpi_file_manager");
}

/**
 * TODO: Write test case for flight number
 */

#endif //NVIDIA_RPI_FILE_MANAGER_TESTER_H
