cmake_minimum_required(VERSION 3.14)

project(robot_controller VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
find_package(mtp32 REQUIRED)
find_package(spdlog REQUIRED)

include(CTest)

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

# --- Locate pigpio -------------------------------------------------------------

# Find pigpio header
find_path(PIGPIO_INCLUDE_DIR
    NAMES pigpio.h
    PATHS /usr/include /usr/local/include
    REQUIRED
)

# Find pigpio libraries
find_library(PIGPIO_LIBRARY
    NAMES pigpio
    PATHS /usr/lib /usr/local/lib
    REQUIRED
)

find_library(PIGPIOD_IF2_LIBRARY
    NAMES pigpiod_if2
    PATHS /usr/lib /usr/local/lib
    REQUIRED
)

# Expose variables to subdirectories
set(PIGPIO_FOUND TRUE)
set(PIGPIO_INCLUDE_DIR ${PIGPIO_INCLUDE_DIR} CACHE STRING "Path to pigpio include directory")
set(PIGPIO_LIBRARIES ${PIGPIO_LIBRARY} ${PIGPIOD_IF2_LIBRARY} pthread CACHE STRING "pigpio libraries")

message(STATUS "pigpio include dir: ${PIGPIO_INCLUDE_DIR}")
message(STATUS "pigpio libraries:   ${PIGPIO_LIBRARIES}")

# --- End of pigpio search ------------------------------------------------------

add_subdirectory(sensor_manager)
add_subdirectory(motor_driver)
add_subdirectory(motion_controller)
add_subdirectory(link_manager)
add_subdirectory(component_manager)
add_subdirectory(robot_middleware)
add_subdirectory(remote_control_agent)
add_subdirectory(src)
add_subdirectory(diagnostics)
