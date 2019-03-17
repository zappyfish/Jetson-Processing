//
// Created by Liam Kelly on 2/26/19.
//

#include "vector_3.h"


Vector3::Vector3() {}

Vector3::~Vector3() {}

void Vector3::copy_vals(Vector3 &to_copy) {
    x = to_copy.x;
    y = to_copy.y;
    z = to_copy.z;
}