//
// Created by Liam Kelly on 3/13/19.
//

#ifndef PROJECT_DUMMY_GPIO_H
#define PROJECT_DUMMY_GPIO_H

#ifndef __linux__
#include "gpio.h"
#include <iostream>

class dummy_gpio : public gpio {

public:

    dummy_gpio() {

    }

    ~dummy_gpio() {

    }

    void set_pin_state(state ste) {
        std::cout << "setting state\n";
    }
    state read_pin_state() {
        std::cout << "reading state\n";
        return gpio::state::low;
    }

private:

    void set_pin_direction(direction dir) {

    }

};

#endif

#endif //PROJECT_DUMMY_GPIO_H
