//
// Created by Liam Kelly on 2/26/19.
//

#ifndef PROJECT_ACCEL_MMA_8451_PI_H
#define PROJECT_ACCEL_MMA_8451_PI_H

#ifdef __linux__

#include "lpf_accel.h"
#include "mma8451_pi.h"

class accel_mma8451_pi : public lpf_accel {

public:

    accel_mma8451_pi(int i2c_dev, float alpha);
    ~accel_mma8451_pi();

private:

    void get_sample(Vector3 &vect);

    mma8451 m_sensor;
};

#endif


#endif //PROJECT_ACCEL_MMA_8451_PI_H
