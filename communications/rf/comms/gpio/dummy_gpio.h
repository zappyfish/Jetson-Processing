//
// Created by Liam Kelly on 3/13/19.
//

#ifndef PROJECT_DUMMY_GPIO_H
#define PROJECT_DUMMY_GPIO_H

#include "uav_gpio.h"
#include <iostream>

class dummy_gpio : public uav_gpio {

public:

    dummy_gpio() {

    }

    ~dummy_gpio() {

    }

    void set_pin_state(uav_gpio::state ste) {
        std::cout << "setting state\n";
    }
    state read_pin_state() {
        std::cout << "reading state\n";
        return uav_gpio::state::low;
    }

private:

    void set_pin_direction(uav_gpio::direction dir) {

    }

};

#endif //PROJECT_DUMMY_GPIO_H
