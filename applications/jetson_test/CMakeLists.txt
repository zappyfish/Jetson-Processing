 cmake_minimum_required(VERSION 2.8)

 if(UNIX AND NOT APPLE)
     set(LINUX TRUE)
 endif()

 project( main )
 set (CMAKE_CXX_STANDARD 11)
 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
 find_package( OpenCV REQUIRED )
 find_package(Boost COMPONENTS system filesystem REQUIRED)
 find_package(Threads REQUIRED)


 include_directories(
         ${OpenCV_INCLUDE_DIRS}
         ${Boost_INCLUDE_DIR}
         ${CMAKE_SOURCE_DIR}/../../communications/pixhawk/packets
         ${CMAKE_SOURCE_DIR}/../../communications/pixhawk/uart
         ${CMAKE_SOURCE_DIR}/../../images
         ${CMAKE_SOURCE_DIR}/../../images/buffer
         ${CMAKE_SOURCE_DIR}/../../images/capture
         ${CMAKE_SOURCE_DIR}/../../logging
         ${CMAKE_SOURCE_DIR}/../../logging/files
         ${CMAKE_SOURCE_DIR}/../../logging/time
 )

 file(GLOB SOURCE
         ../../communications/pixhawk/packets/*.cpp
         ../../communications/pixhawk/uart/jetson_uart.cpp
         ../../communications/pixhawk/uart/uart.cpp
         ../../communications/rf/*.cpp
         ../../images/*.cpp
         ../../images/buffer/*.cpp
         ../../images/capture/camera.cpp
         ../../images/capture/jetson_camera.cpp
         ../../images/processing/*.cpp
         ../../logging/*.cpp
         ../../logging/files/*.cpp
         ../../logging/time/*.cpp
         ../../communications/pixhawk/packets/*.h
         ../../communications/pixhawk/uart/jetson_uart.h
         ../../communications/pixhawk/uart/uart.h
         ../../communications/rf/*.h
         ../../images/*.h
         ../../images/buffer/*.h
         ../../images/capture/camera.h
         ../../images/capture/jetson_camera.h
         ../../images/processing/*.h
         ../../logging/*.h
         ../../logging/files/*.h
         ../../logging/time/*.h
 )

 add_executable( main main.cpp ${SOURCE})

 target_link_libraries( main ${OpenCV_LIBS} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

