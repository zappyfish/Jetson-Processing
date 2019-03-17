//
// Created by Liam Kelly on 11/8/18.
//

#ifndef NVIDIA_FILE_MANAGER_H
#define NVIDIA_FILE_MANAGER_H

#include <cstddef>
#include "image.h"
#include <string>
#include <memory>

typedef std::shared_ptr<image> image_ptr;

class file_manager {

public:

    virtual ~file_manager() {}

    virtual size_t get_flight_number() = 0;

    virtual void write_line_to_file(std::string file_name, std::string line) = 0;
    virtual void write_image(image_ptr img) = 0;
    /**
     * This method exists to be more efficient so we don't have to constantly ask the OS for a file handle.
     * @param file_name
     * @param lines
     * @param num_lines
     */
    virtual void multiline_write(std::string file_name, std::string lines[], size_t num_lines) = 0;
    virtual void multiline_write(std::string file_name, std::vector<std::string> lines) = 0;

private:

    virtual void create_flight_info() = 0;

protected:

    static const std::string FLIGHT_INFO_DOC;

};


#endif //NVIDIA_FILE_MANAGER_H
