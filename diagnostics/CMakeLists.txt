set(COMPONENT "diagnostics")

file(GLOB SRC_FILES
    "*.cpp"
    "*.h"
)

add_executable(${COMPONENT} ${SRC_FILES})
target_link_libraries(${COMPONENT} PUBLIC motion_controller motor_driver spdlog::spdlog ${PIGPIO_LIBRARIES})
target_include_directories(${COMPONENT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})