//
// Created by Liam Kelly on 10/27/18.
//

#include "arducam.h"

arducam::arducam() {}

arducam::~arducam() {}

// Implement me!
image_ptr arducam::capture_image() {
    return std::make_shared<image>();
}