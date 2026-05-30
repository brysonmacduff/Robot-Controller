cmake_minimum_required(VERSION 3.14)

project(robot_controller VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
find_package(mtp32 REQUIRED)
find_package(spdlog REQUIRED)

# --- Locate RF24 -------------------------------------------------------------

# Find the RF24 shared library
find_library(RF24_LIB
    NAMES rf24
    PATHS /usr/local/lib
    REQUIRED
)

# Find the RF24 include directory
find_path(RF24_INCLUDE_DIR
    NAMES RF24/RF24.h
    PATHS /usr/local/include
    REQUIRED
)

# Expose variables to subdirectories
set(RF24_FOUND TRUE)
set(RF24_LIB ${RF24_LIB} CACHE STRING "Path to RF24 library")
set(RF24_INCLUDE_DIR ${RF24_INCLUDE_DIR} CACHE STRING "Path to RF24 include directory")

message(STATUS "RF24 include dir: ${RF24_INCLUDE_DIR}")
message(STATUS "RF24 library:     ${RF24_LIB}")

# -- End of RF24 search ----------------------------------------------------------

add_subdirectory(sensor_manager)
add_subdirectory(motor_driver)
add_subdirectory(track_chassis_controller)
add_subdirectory(link_manager)
add_subdirectory(component_manager)
add_subdirectory(remote_control_protocol)
add_subdirectory(src)