 cmake_minimum_required(VERSION 2.8)

 project( rpi_main )
 set (CMAKE_CXX_STANDARD 11)

 find_library(LIBSOC libsoc.so)

 include_directories(
         ${CMAKE_SOURCE_DIR}/../../communications/rf/comms/gpio
         ${CMAKE_SOURCE_DIR}/../../communications/rf/comms/spi
         ${CMAKE_SOURCE_DIR}/../../communications/rf/packets
         ${CMAKE_SOURCE_DIR}/../../communications/rf
         ${CMAKE_SOURCE_DIR}/../../communications/pixhawk/packets
         ${CMAKE_SOURCE_DIR}/../../communications/pixhawk/uart
 )

 file(GLOB SOURCE
         ../../communications/pixhawk/packets/*.cpp
         ../../communications/pixhawk/uart/uart.cpp
         ./../communications/pixhawk/uart/rpi_uart.cpp
         ../../communications/rf/*.cpp
         ../../communications/rf/comms/gpio/*.cpp
         ../../communications/rf/comms/spi/*.cpp
         ../../communications/rf/packets/*.cpp
         ../../communications/pixhawk/packets/*.h
         ../../communications/pixhawk/uart/uart.h
         ./../communications/pixhawk/uart/rpi_uart.h
         ../../communications/rf/*.h
         ../../communications/rf/comms/gpio/*.h
         ../../communications/rf/comms/spi/*.h
         ../../communications/rf/packets/*.h
         )

 add_executable( rpi_main rpi_main.cpp ${SOURCE})

 target_link_libraries( rpi_main ${LIBSOC})