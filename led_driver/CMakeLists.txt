set(COMPONENT "led_driver")

file(GLOB CODE_FILES
    "*.cpp"
    "*.h"
)

add_library(${COMPONENT} STATIC ${CODE_FILES})
target_include_directories(${COMPONENT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${COMPONENT} PUBLIC ${PIGPIO_LIBRARIES})

add_subdirectory(test_tools)
