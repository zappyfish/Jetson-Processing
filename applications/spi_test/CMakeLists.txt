 cmake_minimum_required(VERSION 2.8)

 if(UNIX AND NOT APPLE)
     set(LINUX TRUE)
 endif()

 project( rpi_main )
 set (CMAKE_CXX_STANDARD 11)



 include_directories(
         ${CMAKE_SOURCE_DIR}/../../communications/rf/comms/gpio
         ${CMAKE_SOURCE_DIR}/../../communications/rf/comms/spi
         ${CMAKE_SOURCE_DIR}/../../communications/rf/packets
         ${CMAKE_SOURCE_DIR}/../../communications/rf/comms/gpio
         ${CMAKE_SOURCE_DIR}/../../communications/rf/comms/spi
         ${CMAKE_SOURCE_DIR}/../../communications/rf/packets
         ${CMAKE_SOURCE_DIR}/../../logging
         ${CMAKE_SOURCE_DIR}/../../logging/files
         ${CMAKE_SOURCE_DIR}/../../logging/time
         ${CMAKE_SOURCE_DIR}/../../images
 )

 file(GLOB SOURCE=
         ../../communications/rf/*.cpp
         ../../communications/rf/comms/gpio/*.cpp
         ../../communications/rf/comms/spi/*.cpp
         ../../communications/rf/packets/*.cpp
         ../../communications/rf/comms/gpio/*.cpp
         ../../communications/rf/comms/spi/*.cpp
         ../../images/image.cpp
         ../../communications/rf/*.h
         ../../communications/rf/comms/gpio/*.h
         ../../communications/rf/comms/spi/*.h
         ../../communications/rf/packets/*.h
         ../../communications/rf/comms/gpio/*.h
         ../../communications/rf/comms/spi/*.h
         ../../images/image.h
         )

 add_executable( rpi_main rpi_main.cpp ${SOURCE})

 target_link_libraries( rpi_main librf24.so )
