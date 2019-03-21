 cmake_minimum_required(VERSION 2.8)

 if(UNIX AND NOT APPLE)
     set(LINUX TRUE)
 endif()

 project( main )
 set (CMAKE_CXX_STANDARD 11)
 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
 find_package( OpenCV REQUIRED )
 find_package(Boost COMPONENTS system filesystem REQUIRED)

 if(LINUX)
     find_package(OpenCV)
     find_library(LIBSOC libsoc.so)
     IF  ( OpenCV_FOUND)
         MESSAGE(STATUS "FOUND OPENCV")
     ENDIF()

     # Locate libraries and headers
     find_package(Threads REQUIRED)

 endif()


 include_directories(
         ${OpenCV_INCLUDE_DIRS}
         ${Boost_INCLUDE_DIR}
         ${CMAKE_SOURCE_DIR}/../../communications/pixhawk/packets
         ${CMAKE_SOURCE_DIR}/../../communications/pixhawk/uart
         ${CMAKE_SOURCE_DIR}/../../communications/rf/comms/gpio
         ${CMAKE_SOURCE_DIR}/../../communications/rf/comms/spi
         ${CMAKE_SOURCE_DIR}/../../communications/rf/packets
         ${CMAKE_SOURCE_DIR}/../../images
         ${CMAKE_SOURCE_DIR}/../../images/buffer
         ${CMAKE_SOURCE_DIR}/../../images/capture
         ${CMAKE_SOURCE_DIR}/../../images/processing
         ${CMAKE_SOURCE_DIR}/../../images/processing/target
         ${CMAKE_SOURCE_DIR}/../../images/processing/uav/accel
         ${CMAKE_SOURCE_DIR}/../../logging
         ${CMAKE_SOURCE_DIR}/../../logging/files
         ${CMAKE_SOURCE_DIR}/../../logging/time
 )

 file(GLOB SOURCE
         ../../communications/pixhawk/packets/*.cpp
         ../../communications/pixhawk/uart/jetson_uart.cpp
         ../../communications/pixhawk/uart/uart.cpp
         ../../communications/rf/*.cpp
         ../../communications/rf/comms/gpio/*.cpp
         ../../communications/rf/comms/spi/*.cpp
         ../../communications/rf/packets/*.cpp
         ../../images/*.cpp
         ../../images/buffer/*.cpp
         ../../images/capture/jetson_camera.cpp
         ../../images/capture/camera.cpp
         ../../images/processing/*.cpp
         ../../images/processing/target/*.cpp
         ../../images/processing/uav/accel/*.cpp
         ../../images/processing/uav/accel/*.c
         ../../logging/*.cpp
         ../../logging/files/*.cpp
         ../../logging/time/*.cpp
         ../../communications/pixhawk/packets/*.h
         ../../communications/pixhawk/uart/jetson_uart.h
         ../../communications/pixhawk/uart/uart.h
         ../../communications/rf/*.h
         ../../communications/rf/comms/gpio/*.h
         ../../communications/rf/comms/spi/*.h
         ../../communications/rf/packets/*.h
         ../../images/*.h
         ../../images/buffer/*.h
         ../../images/capture/jetson_camera.h
         ../../images/capture/camera.h
         ../../images/processing/*.h
         ../../images/processing/target/*.h
         ../../images/processing/uav/accel/*.h
         ../../logging/*.h
         ../../logging/files/*.h
         ../../logging/time/*.h
         )

 add_executable( main main.cpp ${SOURCE})

 if(LINUX)
     target_link_libraries( main ${OpenCV_LIBS} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${LIBSOC})
 else()
     target_link_libraries( main ${OpenCV_LIBS} ${Boost_LIBRARIES})
 endif()
