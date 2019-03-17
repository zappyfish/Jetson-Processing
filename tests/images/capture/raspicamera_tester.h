//
// Created by Liam Kelly on 11/11/18.
//

#ifndef NVIDIA_RASPICAMERA_TESTER_H
#define NVIDIA_RASPICAMERA_TESTER_H

#ifdef __linux__

#include "raspicamera.h"
#include "data_logger.h"
#include "rpi_file_manager.h"

void rpi_cam_setup();
void rpi_cam_teardown();
void rpi_cam_fill_file_list(std::vector<std::string> &file_list, const std::string& path);

const std::string RPI_CAM_TEST_DIRECTORY = "test_directory/raspicam";

TEST_CASE("Raspicamera Test Capture Image", "[raspicamera]") {
    rpi_cam_setup();

    raspicamera camera;
    image_ptr img = camera.capture_image();

    rpi_file_manager *f_manager = new rpi_file_manager(RPI_CAM_TEST_DIRECTORY);
    data_logger::get_instance().start_flight_session(f_manager);

    data_logger::get_instance().log_image(*img);

    std::string saved_image_path = RPI_CAM_TEST_DIRECTORY + "/vadl/flights/flight_0/images/";
    std::vector<std::string> file_list;
    rpi_cam_fill_file_list(file_list, saved_image_path);

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

    rpi_cam_teardown();
}

// ------------------------ Helper functions ----------------------- //

void rpi_cam_fill_file_list(std::vector<std::string> &file_list, const std::string& path) {
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

void rpi_cam_setup() {
    rpi_cam_teardown();
    if (!exists("test_directory")) {
        create_directory("test_directory");
    }
    create_directory("test_directory/raspicam");
}

void rpi_cam_teardown() {
    remove_all("test_directory/raspicam");
}

#endif

#endif //NVIDIA_RASPICAMERA_TESTER_H
