//
// Created by Liam Kelly on 1/22/19.
//

#include "flight_system.h"

flight_system::flight_system() {}

flight_system::~flight_system() {}

void flight_system::init_system() {
    setup_packet_manager();
}

void flight_system::start_flight() {
    flight_setup();

    perform_flight();

    flight_teardown();
}

