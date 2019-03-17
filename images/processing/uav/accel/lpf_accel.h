//
// Created by Liam Kelly on 2/26/19.
//

#ifndef PROJECT_ACCEL_H
#define PROJECT_ACCEL_H

#include "vector_3.h"

class lpf_accel {

public:

    lpf_accel(float alpha);
    ~lpf_accel();

    void get_accel(Vector3 &vect);

private:

    virtual void get_sample(Vector3 &vect) = 0;
    void lpf_sample(Vector3 &sample);

    float m_alpha;

    Vector3 m_last_filtered_sample;
};


#endif //PROJECT_ACCEL_H
