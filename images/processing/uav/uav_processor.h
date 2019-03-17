//
// Created by Liam Kelly on 10/27/18.
//

#ifndef NVIDIA_UAV_PROCESSOR_H
#define NVIDIA_UAV_PROCESSOR_H

/**
 * Abstract class
 */
class uav_processor {

public:

    uav_processor();
    ~uav_processor();

    void get_localization(cv::Mat &img, double *pose, double *position);


private:

    double m_euler_pose[3];
    double m_position[3];

};


#endif //NVIDIA_UAV_PROCESSOR_H
