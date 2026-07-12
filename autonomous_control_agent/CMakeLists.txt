set(COMPONENT "autonomous_control_agent")

file(GLOB SRC_FILES
    "*.cpp"
    "*.h"
)

add_library(${COMPONENT} STATIC ${SRC_FILES})
target_include_directories(${COMPONENT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${COMPONENT} PUBLIC spdlog::spdlog onnxruntime)

add_subdirectory(test)
add_subdirectory(test_tools)
