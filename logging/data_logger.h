//
// Created by Liam Kelly on 11/7/18.
//

#ifndef NVIDIA_DATA_LOGGER_H
#define NVIDIA_DATA_LOGGER_H

#include "log_entry.h"
#include "file_manager.h"
#include "image.h"
#include <mutex>
#include <thread>

typedef std::shared_ptr<image> image_ptr;

class data_logger {

public:
    static const size_t ENTRY_BUFFER_SIZE = 1000; // After we reach this many entries in memory, log them.
    // We do this because we don't want to be doing frequent SD card I/O because it's slow,
    // but we also don't want to take too much memory storing log entries; this
    // is a compromise.

    static data_logger& get_instance();

    ~data_logger();
    /**
     * Can call this whenever
     * @param entry
     */
    void save_log_entry(log_entry *entry);

    /**
     * Can call this whenever
     * @param img
     */
    void log_image(image_ptr img);

    /**
     * Give the data_logger a file_manager
     * @param f_manager
     */
    void start_flight_session(file_manager *f_manager);

    void end_flight_session();

    /**
     * Note: this method will block until completion
     */
    void write_remaining_entries();


private:

    data_logger();

    void write_entries();

    log_entry **m_entry_buffer;

    // The logging buffer takes on the reference of the entry_buffer once it's time for logging
    log_entry **m_logging_buffer;

    file_manager *m_file_manager;

    size_t m_num_entries;
    std::mutex m_buffer_lock;
    std::mutex m_write_lock;
    std::mutex m_session_change_lock;

    std::thread *m_io_thread;
};


#endif //NVIDIA_DATA_LOGGER_H
