set(COMPONENT ${PROJECT_NAME})

file(GLOB SRC_FILES
    "*.cpp"
    "*.h"
)

add_executable(${COMPONENT} ${SRC_FILES})
target_link_libraries(${COMPONENT} PUBLIC component_manager)
target_include_directories(${COMPONENT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Use standard install directories
include(GNUInstallDirs)

# ----------------------------------------
# Install the executable
# ----------------------------------------
install(
    TARGETS ${COMPONENT}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
