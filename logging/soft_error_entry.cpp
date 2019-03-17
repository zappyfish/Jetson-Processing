//
// Created by Liam Kelly on 11/8/18.
//

#include "soft_error_entry.h"

const std::string soft_error_entry::ENTRY_ID = "SOFT_ERROR";

soft_error_entry::soft_error_entry(std::string category, std::string description,
        std::map<std::string, std::string> metadata) : log_entry() {

    add_data(std::string("category"), category);
    add_data(std::string("description"), description);
    add_data(std::string("metadata_length"), static_cast<int>(metadata.size()));

    for (auto const &data_pair: metadata) {
        add_data(data_pair.first, data_pair.second);
    }
}

std::string soft_error_entry::get_name() {
    return soft_error_entry::ENTRY_ID;
}