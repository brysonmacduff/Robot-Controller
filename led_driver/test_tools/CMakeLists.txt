set(TOOL_TARGET ${COMPONENT}.tool)

file(GLOB CODE_FILES
    "*.cpp"
    "*.h"
)

add_executable(${TOOL_TARGET} ${CODE_FILES})
target_include_directories(${TOOL_TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${TOOL_TARGET} PUBLIC ${COMPONENT} spdlog::spdlog)

