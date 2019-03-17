//
// Created by Liam Kelly on 11/7/18.
//

#include "data_logger.h"

data_logger& data_logger::get_instance() {
    static data_logger instance;
    return instance;
}

data_logger::data_logger() : m_num_entries(0), m_logging_buffer(new log_entry*[data_logger::ENTRY_BUFFER_SIZE]),
m_entry_buffer(new log_entry*[data_logger::ENTRY_BUFFER_SIZE]), m_file_manager(nullptr), m_io_thread(nullptr) {
    for (size_t i = 0; i < data_logger::ENTRY_BUFFER_SIZE; i++) {
        m_entry_buffer[i] = m_logging_buffer[i] = nullptr;
    }
}

data_logger::~data_logger() {
    end_flight_session();

    // Cleanup
    delete[] m_logging_buffer;
    delete[] m_entry_buffer;
}

void data_logger::start_flight_session(file_manager *f_manager) {
    end_flight_session();
    m_file_manager = f_manager;
}

void data_logger::end_flight_session() {
    m_session_change_lock.lock();
    if (m_io_thread != nullptr) {
        m_io_thread->join();
        delete m_io_thread;
        m_io_thread = nullptr;
    }
    if (m_file_manager != nullptr) {
        write_remaining_entries();
        delete m_file_manager;
        m_file_manager = nullptr;
    }
    m_session_change_lock.unlock();
}

void data_logger::log_image(image_ptr img) {
    if (m_file_manager != nullptr) {
        m_file_manager->write_image(img);
    }
}

void data_logger::save_log_entry(log_entry *entry) {
    m_buffer_lock.lock();
    m_entry_buffer[m_num_entries++] = entry;
    if (m_num_entries >= data_logger::ENTRY_BUFFER_SIZE) {
        m_num_entries = 0;
        // Should only ever swap buffers if you're not writing
        m_write_lock.lock();
        // Swap buffers
        log_entry **temp = m_logging_buffer;
        m_logging_buffer = m_entry_buffer;
        m_entry_buffer = temp;
        // Start thread
        if (m_io_thread != nullptr) {
            m_io_thread->join();
            delete m_io_thread;
        }
        m_io_thread = new std::thread(&data_logger::write_entries, this);
    }
    m_buffer_lock.unlock();
}

void data_logger::write_entries() {
    if (m_file_manager != nullptr) {
        std::map <std::string, std::vector<std::string>> file_name_lines_map;
        for (size_t i = 0; i < data_logger::ENTRY_BUFFER_SIZE; i++) {
            log_entry *next_entry = m_logging_buffer[i];
            if (next_entry == nullptr) break; // Might be a destructor write, therefore buffer might not be full
            // Check to see if this type of entry exists in the map
            if (file_name_lines_map.find(next_entry->get_name()) == file_name_lines_map.end()) {
                std::vector <std::string> lines;
                file_name_lines_map[next_entry->get_name()] = lines;
            }
            file_name_lines_map[next_entry->get_name()].push_back(next_entry->get_log_line());

            delete next_entry; // Cleanup, we got what we needed
            m_logging_buffer[i] = nullptr;
        }
        for (auto const &file_line_pair: file_name_lines_map) {
            std::string file_name = file_line_pair.first + ".txt";
            m_file_manager->multiline_write(file_name, file_line_pair.second);
        }
    }
    m_write_lock.unlock();
}

void data_logger::write_remaining_entries() {
    // Need to reset the buffer entries to 0 and swap since we're going to write, and to do this we need a lock on the buffer.
    m_buffer_lock.lock();
    m_num_entries = 0;
    // Should only ever swap buffers if you're not writing
    m_write_lock.lock();

    // Swap buffers
    log_entry **temp = m_logging_buffer;
    m_logging_buffer = m_entry_buffer;
    m_entry_buffer = temp;

    // Safe to use the buffer again
    m_buffer_lock.unlock();

    // Write any final data
    write_entries();
}