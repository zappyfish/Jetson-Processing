 cmake_minimum_required(VERSION 2.8)

 if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
 endif()

 project( log_stuff )
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
         ${CMAKE_SOURCE_DIR}/../../images
         ${CMAKE_SOURCE_DIR}/../../images/buffer
         ${CMAKE_SOURCE_DIR}/../../images/capture
         ${CMAKE_SOURCE_DIR}/../../logging
         ${CMAKE_SOURCE_DIR}/../../logging/files
         ${CMAKE_SOURCE_DIR}/../../logging/time
 )

 file(GLOB SOURCE
         ../../communications/pixhawk/*.cpp
         ../../communications/rf/*.cpp
         ../../images/*.cpp
         ../../images/buffer/*.cpp
         ../../images/capture/*.cpp
         ../../images/processing/*.cpp
         ../../logging/*.cpp
         ../../logging/files/*.cpp
         ../../logging/time/*.cpp
         ../../communications/pixhawk/*.h
         ../../communications/rf/*.h
         ../../images/*.h
         ../../images/buffer/*.h
         ../../images/capture/*.h
         ../../images/processing/*.h
         ../../logging/*.h
         ../../logging/files/*.h
         ../../logging/time/*.h
         )

 add_executable( log_stuff log_stuff.cpp ${SOURCE})

 if(LINUX)
  target_link_libraries( log_stuff ${OpenCV_LIBS} ${Boost_LIBRARIES} ${raspicam_CV_LIBS})
 else()
  target_link_libraries( log_stuff ${OpenCV_LIBS} ${Boost_LIBRARIES})
 endif()
