//
// Created by Liam Kelly on 2/7/19.
//

#include "subscale_2_system.h"
#include "packet_manager.h"
#include <chrono>
#include <thread>
#include <iostream>

#define PACKET_WAIT_MS 10 // Check for packets from pixhawk every 10 ms

void packet_check();

int main() {

    subscale2_system system;
    system.init_system();
    std::thread packet_check_thread(packet_check);
    while (true) {
        while (!system.should_takeoff()) {
            // Check in every PACKET_WAIT_MS to see if we should take off
            std::this_thread::sleep_for(std::chrono::milliseconds(PACKET_WAIT_MS));
        }
        std::cout << "Time for takeoff!\n";
        system.start_flight();
        std::cout << "Flight complete!\n";
    }

    return 0;
}

void packet_check() {
    while (true) {
        packet_manager::get_instance().check_packets();
        std::this_thread::sleep_for(std::chrono::milliseconds(PACKET_WAIT_MS));
    }
}