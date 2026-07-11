set(COMPONENT "configuration")

file(GLOB SRC_FILES
    "*.cpp"
    "*.h"
)

add_library(${COMPONENT} STATIC ${SRC_FILES})
target_include_directories(${COMPONENT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${COMPONENT} PUBLIC yaml-cpp spdlog::spdlog)

add_subdirectory(test)
