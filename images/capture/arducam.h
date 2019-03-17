//
// Created by Liam Kelly on 10/27/18.
//

#ifndef NVIDIA_ARDUCAM_H
#define NVIDIA_ARDUCAM_H

#include "camera.h"

class arducam : public camera {

public:

    arducam();
    ~arducam();

private:
    image_ptr capture_image();

};


#endif //NVIDIA_ARDUCAM_H
