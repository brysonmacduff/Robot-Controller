cmake_minimum_required(VERSION 3.14)

project(robot_controller VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
find_package(mtp32 REQUIRED)

add_subdirectory(sensor_manager)
add_subdirectory(motor_driver)
add_subdirectory(track_chassis_controller)
add_subdirectory(link_manager)
add_subdirectory(component_manager)
add_subdirectory(remote_control_protocol)
add_subdirectory(src)