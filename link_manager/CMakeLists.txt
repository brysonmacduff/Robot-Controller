set(COMPONENT "link_manager")

file(GLOB SRC_FILES
    "*.cpp"
    "*.h"
)

add_library(${COMPONENT} STATIC ${SRC_FILES})
target_link_libraries(${COMPONENT} PUBLIC remote_control_protocol mtp32::mtp32 ${RF24_LIB} spdlog::spdlog)
target_include_directories(${COMPONENT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(test)
