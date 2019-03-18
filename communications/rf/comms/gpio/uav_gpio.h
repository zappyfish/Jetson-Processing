//
// Created by Liam Kelly on 2/10/19.
//

#ifndef PROJECT_GPIO_H
#define PROJECT_GPIO_H

#include <iostream>

class uav_gpio {

public:

    virtual ~uav_gpio() {}

    enum direction {
        output = 1,
        input = 0
    };

    enum state {
        high = 1,
        low = 0
    };

    virtual void set_pin_state(state ste) = 0;
    virtual state read_pin_state() = 0;

private:

    virtual void set_pin_direction(direction dir) = 0;
};


#endif //PROJECT_GPIO_H
