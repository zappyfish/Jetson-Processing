cmake_minimum_required(VERSION 3.10)

 if(UNIX AND NOT APPLE)
     set(LINUX TRUE)
 endif()

 project(tests)
 set (CMAKE_CXX_STANDARD 11)
 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
 find_package( OpenCV REQUIRED )
 find_package(Boost COMPONENTS system filesystem REQUIRED)

 if(LINUX)
    set(raspicam_DIR "/usr/local/lib/cmake")
    find_package(raspicam REQUIRED)
 endif()

 include_directories(
         ${OpenCV_INCLUDE_DIRS}
         ${Boost_INCLUDE_DIR}
         ${CMAKE_SOURCE_DIR}/../communications/pixhawk/packets
         ${CMAKE_SOURCE_DIR}/../communications/pixhawk/uart
         ${CMAKE_SOURCE_DIR}/../images
         ${CMAKE_SOURCE_DIR}/../images/buffer
         ${CMAKE_SOURCE_DIR}/../images/capture
         ${CMAKE_SOURCE_DIR}/../images/processing/target
         ${CMAKE_SOURCE_DIR}/../images/processing/uav/accel
         ${CMAKE_SOURCE_DIR}/../logging
         ${CMAKE_SOURCE_DIR}/../logging/files
         ${CMAKE_SOURCE_DIR}/../logging/time
         ${CMAKE_SOURCE_DIR}/../communications/rf/packets
         ${CMAKE_SOURCE_DIR}/./communications/pixhawk/packets
         ${CMAKE_SOURCE_DIR}/./communications/pixhawk/uart
         ${CMAKE_SOURCE_DIR}/./images
         ${CMAKE_SOURCE_DIR}/./images/buffer
         ${CMAKE_SOURCE_DIR}/./images/capture
         ${CMAKE_SOURCE_DIR}/./images/processing/target
         ${CMAKE_SOURCE_DIR}/./images/processing/uav/accel
         ${CMAKE_SOURCE_DIR}/./logging
         ${CMAKE_SOURCE_DIR}/./logging/files
         ${CMAKE_SOURCE_DIR}/./logging/time
 )

 file(GLOB SOURCE
         ../communications/pixhawk/packets/*.cpp
         ../communications/pixhawk/uart/*.cpp
         ../communications/rf/*.cpp
         ../communications/rf/packets*.cpp
         ../images/*.cpp
         ../images/buffer/*.cpp
         ../images/capture/*.cpp
         ../images/processing/*.cpp
         ../images/processing/target/*.cpp
         ../images/processing/uav/accel/*.cpp
         ../logging/*.cpp
         ../logging/files/*.cpp
         ../logging/time/*.cpp
         ../communications/pixhawk/packets/*.h
         ../communications/pixhawk/uart/*.h
         ../communications/rf/*.h
         ../communications/rf/packets*.h
         ../images/*.h
         ../images/buffer/*.h
         ../images/capture/*.h
         ../images/processing/*.h
         ../images/processing/target/*.h
         ../images/processing/uav/accel/*.h
         ../logging/*.h
         ../logging/files/*.h
         ../logging/time/*.h
 )

 add_executable(tests tests.cpp ${SOURCE})

 if(LINUX)
    target_link_libraries(tests ${OpenCV_LIBS} ${Boost_LIBRARIES} ${raspicam_CV_LIBS})
 else()
     target_link_libraries(tests ${OpenCV_LIBS} ${Boost_LIBRARIES})
 endif()
