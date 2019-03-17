//
// Created by Liam Kelly on 1/22/19.
//

#ifndef PROJECT_FLIGHT_SYSTEM_H
#define PROJECT_FLIGHT_SYSTEM_H

#include "data_logger.h"

class flight_system {

public:

    flight_system();
    ~flight_system();
    
    /**
     * Must call this method once at start of application to prepare systems
     */
    void init_system();

    /**
     * Call this when ready for take off
     */
    void start_flight();

private:

    /**
     * Set UART and necessary pre-flight packet callbacks here
     */
    virtual void setup_packet_manager() = 0;

    /**
     * This method will take a vector and bring it through the transform experienced by the camera. In other words,
     * it will take a 3-vector and transform it from the reference frame of the camera to the reference frame which looks
     * directly below the UAV
     */
    virtual void perform_camera_transform(double *v) = 0;

    /**
     * Assumes setup/init is complete
     */
     virtual void perform_flight() = 0;

     /**
      * Any additional setup work
      */

     virtual void flight_setup() = 0;

     /**
      * Any additional teardown work
      */

     virtual void flight_teardown() = 0;


};


#endif //PROJECT_FLIGHT_SYSTEM_H
