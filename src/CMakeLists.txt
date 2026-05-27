set(COMPONENT ${PROJECT_NAME})

file(GLOB SRC_FILES
    "*.cpp"
    "*.h"
)

add_executable(${COMPONENT} ${SRC_FILES})
target_include_directories(${COMPONENT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
