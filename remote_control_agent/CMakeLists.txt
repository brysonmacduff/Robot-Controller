set(COMPONENT "remote_control_agent")

file(GLOB SRC_FILES
    "*.cpp"
    "*.h"
)

add_library(${COMPONENT} STATIC ${SRC_FILES})
target_link_libraries(${COMPONENT} PUBLIC link_manager motion_controller robot_middleware)
target_include_directories(${COMPONENT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(test)
