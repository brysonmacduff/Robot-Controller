set(COMPONENT "utilities")

file(GLOB SRC_FILES
    "*.cpp"
    "*.h"
)

add_library(${COMPONENT} STATIC ${SRC_FILES})
target_include_directories(${COMPONENT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(${COMPONENT} PROPERTIES LINKER_LANGUAGE CXX)