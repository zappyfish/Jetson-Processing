//
// Created by Liam Kelly on 11/11/18.
//

#ifndef NVIDIA_RASPICAM_H
#define NVIDIA_RASPICAM_H

#ifdef __linux__

#include "camera.h"
#include <raspicam/raspicam_cv.h>
#include <opencv2/imgproc.hpp>

using namespace std;

class raspicamera : public camera {

public:
    raspicamera();
    ~raspicamera();

    image_ptr capture_image();
private:

    raspicam::RaspiCam_Cv m_raspicam;
};

#endif

#endif //NVIDIA_RASPICAM_H
