//
// Created by Liam Kelly on 11/8/18.
//

#include "rpi_file_manager.h"

const std::string rpi_file_manager::DIRECTORY_PATH[] = {"vadl", "flights"};
const int rpi_file_manager::PATH_LENGTH = 2;

rpi_file_manager::rpi_file_manager(std::string root_dir) : m_flight_path(root_dir) {
    // Make required directories
    for (size_t i = 0; i < PATH_LENGTH; i++) {
        m_flight_path += "/" + DIRECTORY_PATH[i];
        if (!exists(m_flight_path)) {
            create_directory(m_flight_path);
        }
    }
    // Count the number of flight_n directories, yielding the next number
    m_flight_number = std::count_if(
            directory_iterator(path(m_flight_path)),
            directory_iterator(),
            static_cast<bool(*)(const path&)>(is_directory) );
    m_flight_path += "/flight_" + std::to_string(m_flight_number);
    // Create the flight directory
    create_directory(m_flight_path);
    create_directory(m_flight_path + "/images");
    create_flight_info();
}

rpi_file_manager::rpi_file_manager() : rpi_file_manager(".") {

}

rpi_file_manager::~rpi_file_manager() {

}

size_t rpi_file_manager::get_flight_number() {
    return m_flight_number;
}

/**
 *
 * @param file_name: name of file (without path)
 * @param line: should already have \n character
 */
void rpi_file_manager::write_line_to_file(std::string file_name, std::string line) {
    std::string full_path = m_flight_path + "/" + file_name;
    std::ofstream out;
    out.open(full_path, std::ios_base::app);
    out << line;
    out.close();
}

void rpi_file_manager::multiline_write(std::string file_name, std::string lines[], size_t num_lines) {
    std::string full_path = m_flight_path + "/" + file_name;
    std::ofstream out;
    out.open(full_path, std::ios_base::app);

    for (size_t i = 0; i < num_lines; i++) {
        out << lines[i];
    }

    out.close();
}

void rpi_file_manager::multiline_write(std::string file_name, std::vector<std::string> lines) {
    std::string full_path = m_flight_path + "/" + file_name;
    std::ofstream out;
    out.open(full_path, std::ios_base::app);

    for (size_t i = 0; i < lines.size(); i++) {
        out << lines.at(i);
    }

    out.close();
}

void rpi_file_manager::write_image(image_ptr img) {
    std::string full_path = m_flight_path + "/images/" + img->get_timestamp() + ".jpg";
    cv::imwrite(full_path, img->get_original_mat());
}

void rpi_file_manager::create_flight_info() {
    std::string time_info = "t_start:" + std::to_string(std::time(0)) + "\n";
    write_line_to_file(file_manager::FLIGHT_INFO_DOC, time_info);
}