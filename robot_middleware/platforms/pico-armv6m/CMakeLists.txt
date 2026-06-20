cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico_w)
set(PICO_SDK_PATH "../pico-sdk")

# Import Pico SDK (must be before project())
include(pico_sdk_import.cmake)

project(robot_middleware
    VERSION 0.0.0
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)

# ------------------------------------------------------------
# Build robot_middleware for the Pico
# ------------------------------------------------------------

file(GLOB RM_SOURCES
    "../../*.cpp"
)

file(GLOB RM_HEADERS
    "../../*.h"
)

add_library(${PROJECT_NAME} STATIC ${RM_SOURCES})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
        $<INSTALL_INTERFACE:include>
)

# ------------------------------------------------------------
# Installation layout (correct + platform-safe)
# ------------------------------------------------------------
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(RM_INSTALL_INCLUDEDIR "${CMAKE_INSTALL_INCLUDEDIR}/robot_middleware")
set(RM_INSTALL_LIBDIR "${CMAKE_INSTALL_LIBDIR}")
set(RM_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/robot_middleware")

install(TARGETS ${PROJECT_NAME}
    EXPORT robot_middlewareTargets
    ARCHIVE DESTINATION ${RM_INSTALL_LIBDIR}
)

# Install only the public headers
install(FILES ${RM_HEADERS}
    DESTINATION ${RM_INSTALL_INCLUDEDIR}
)

# ------------------------------------------------------------
# Generate CMake package config
# ------------------------------------------------------------
configure_package_config_file(
    ../../cmake/robot_middlewareConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/robot_middlewareConfig.cmake
    INSTALL_DESTINATION ${RM_INSTALL_CMAKEDIR}
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/robot_middlewareConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/robot_middlewareConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/robot_middlewareConfigVersion.cmake
    DESTINATION ${RM_INSTALL_CMAKEDIR}
)

install(EXPORT robot_middlewareTargets
    FILE robot_middlewareTargets.cmake
    NAMESPACE robot_middleware::
    DESTINATION ${RM_INSTALL_CMAKEDIR}
)
