file(GLOB SOURCES
    "*.cpp"
    "*.h"
)

add_executable(${COMPONENT}.tool ${SOURCES})

target_include_directories(${COMPONENT}.tool PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${COMPONENT}.tool PUBLIC ${COMPONENT})
