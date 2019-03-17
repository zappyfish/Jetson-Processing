//
// Created by Liam Kelly on 10/27/18.
//

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "image_buffer_tester.h"
#include "camera_tester.h"
#include "arducam_tester.h"
#include "rpi_file_manager_tester.h"
#include "log_entry_tester.h"
#include "soft_error_entry_tester.h"
#include "time_manager_tester.h"
#include "image_tester.h"
#include "data_logger_tester.h" // TODO: I think this is the one that causes the libc++abi.dylib error. fix.
#include "raspicamera_tester.h"
#include "test_packet_tester.h"
#include "packet_manager_tester.h"
#include "flight_packet_tester.h"
#include "pid_angle_controller_tester.h"
#include "directions_packet_tester.h"
#include "reference_frame_tester.h"
#include "ack_packet_tester.h"

// See https://github.com/catchorg/Catch2/blob/master/docs/tutorial.md for tutorial


