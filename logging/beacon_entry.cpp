//
// Created by Liam Kelly on 4/1/19.
//

#include "beacon_entry.h"

const std::string beacon_entry::ENTRY_ID = "beacon_deploy";

beacon_entry::beacon_entry() {}

beacon_entry::~beacon_entry() {}

std::string beacon_entry::get_name() {
    return beacon_entry::ENTRY_ID;
}