//
// Created by Liam Kelly on 2/26/19.
//

#include "accel_mma_8451_pi.h"

#ifdef __linux__

accel_mma8451_pi::accel_mma8451_pi(float alpha) : lpf_accel(alpha), m_sensor(mma8451_initialise(1, MMA8451_DEFAULT_ADDR)) {
    //configure the max range, can be 2G, 4G or 8G
    mma8451_set_range(&m_sensor, 8); //For a maximum of a 4G acceleartion vector
}

accel_mma8451_pi::~accel_mma8451_pi() {}

void accel_mma8451_pi::get_sample(Vector3 &vect) {
    mma8451_vector3 mma_vect;
    mma8451_get_acceleration(&m_sensor, &mma_vect);
    vect.x = mma_vect.x * 9.8; // Sensor returns values in g's
    vect.y = mma_vect.y * 9.8;
    vect.z = mma_vect.z * 9.8;
}

#endif