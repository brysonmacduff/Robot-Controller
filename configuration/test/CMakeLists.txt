file(GLOB SOURCES
    "*.cpp"
    "*.h"
)

add_executable(${COMPONENT}.test ${SOURCES})

target_include_directories(${COMPONENT}.test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${COMPONENT}.test PUBLIC
    GTest::gtest_main
    ${COMPONENT}
)

# Copy example configuration file to build/configuration/test for inclusion in unit tests
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.yaml ${CMAKE_CURRENT_BINARY_DIR}/config.yaml COPYONLY)

# Copy example configuration file to build/src for use by the robot controller executable
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.yaml ${CMAKE_BINARY_DIR}/src/config.yaml COPYONLY)

add_test(NAME ${COMPONENT}.test COMMAND ${COMPONENT}.test)