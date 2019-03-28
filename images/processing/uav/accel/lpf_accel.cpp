//
// Created by Liam Kelly on 2/26/19.
//

#include "lpf_accel.h"
#include <iostream>

lpf_accel::lpf_accel(float alpha) : m_alpha(alpha) {
    m_last_filtered_sample.x = 0;
    m_last_filtered_sample.y = 0;
    m_last_filtered_sample.z = -9.8; // best guesses (in G's)
}

lpf_accel::~lpf_accel() {}

void lpf_accel::get_accel(Vector3 &vect) {
    get_sample(vect);
    lpf_sample(vect);
}

void lpf_accel::lpf_sample(Vector3 &sample) {
    m_last_filtered_sample.x = m_alpha * (sample.x - m_last_filtered_sample.x) + m_last_filtered_sample.x;
    m_last_filtered_sample.y = m_alpha * (sample.y - m_last_filtered_sample.y) + m_last_filtered_sample.y;
    m_last_filtered_sample.z = m_alpha * (sample.z - m_last_filtered_sample.z) + m_last_filtered_sample.z;

    std::cout << m_last_filtered_sample.x << ", " << m_last_filtered_sample.y << ", " << m_last_filtered_sample.z << std::endl;

    sample.copy_vals(m_last_filtered_sample);
}