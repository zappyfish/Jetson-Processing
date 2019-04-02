 cmake_minimum_required(VERSION 2.8)

 if(UNIX AND NOT APPLE)
     set(LINUX TRUE)
 endif()

 project( main )
 set (CMAKE_CXX_STANDARD 11)



 include_directories(
         ${CMAKE_SOURCE_DIR}/../../communications/rf/comms/gpio
         ${CMAKE_SOURCE_DIR}/../../communications/rf/comms/spi
         ${CMAKE_SOURCE_DIR}/../../communications/rf/packets
 )

 file(GLOB SOURCE=
         ../../communications/rf/*.cpp
         ../../communications/rf/comms/gpio/*.cpp
         ../../communications/rf/comms/spi/*.cpp
         ../../communications/rf/packets/*.cpp
         ../../communications/rf/*.h
         ../../communications/rf/comms/gpio/*.h
         ../../communications/rf/comms/spi/*.h
         ../../communications/rf/packets/*.h
         )

 add_executable( main main.cpp ${SOURCE})

 target_link_libraries( main librf24.so )