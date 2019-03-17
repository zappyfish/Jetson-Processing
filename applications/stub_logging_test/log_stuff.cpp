//
// Created by Liam Kelly on 11/12/18.
//

#include "test_camera.h"
#include "data_logger.h"
#include "time_manager.h"
#include "test_entry.h"
#include "image_buffer.h"
#include "rpi_file_manager.h"

void process();

int main() {

    remove_all("./vadl");
    image_buffer *main_buffer = new image_buffer();
    test_camera camera("./test2.jpg");
    rpi_file_manager *f_manager = new rpi_file_manager();
    data_logger::get_instance().start_flight_session(f_manager);
    camera.start_capture(main_buffer);

    size_t start_time = time_manager::get_instance().get_relative_time();
    // Go for 10 seconds
    size_t time_expired = time_manager::get_instance().get_relative_time() - start_time;
    size_t cnt = 0;
    while (time_expired < 20000) {
        test_entry *entry = new test_entry(8, 8, 8);
        data_logger::get_instance().save_log_entry(entry);
        size_t pre_retrieve = time_manager::get_instance().get_relative_time();
        image_ptr img = main_buffer->retrieve_image();
        std::cout << "time to retrieve was: " << (time_manager::get_instance().get_relative_time() - pre_retrieve) << "ms\n";
        size_t pre_log = time_manager::get_instance().get_relative_time();
        data_logger::get_instance().log_image(*img);
        std::cout << "time to log was: " << (time_manager::get_instance().get_relative_time()  - pre_log) << "ms\n";

        process(); // Pretend we're processing

        time_expired = time_manager::get_instance().get_relative_time() - start_time;
        cnt++;
    }

    std::cout << "captured and processed" << cnt << " images in 20s with buffer\n";
    // Cleanup
    camera.end_capture();
    data_logger::get_instance().end_flight_session();
    delete main_buffer;


    return 0;
}

void process() {
    std::cout << "Processing image...takes about 25 ms\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(25));
}