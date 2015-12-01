# Parse target source code
include(parse_src)

# Cache library includes folder
set(${PROJECT_NAME}_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE INTERNAL "${PROJECT_NAME} library includes" FORCE)

add_definitions(-DQT_PLUGIN)

# Build static library
add_library(${PROJECT_NAME} SHARED ${SRC_CPP} ${SRC_C} ${SRC_H} ${SRC_H_PRIVATE} ${SRC_RES})
target_link_libraries(${PROJECT_NAME} ${DEPENDS})

# Link with dependencies
include(link_libs)

# Process unit tests
include(parse_tests)

# Install this shared library
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${INSTALL_BIN_PATH}
        LIBRARY DESTINATION ${INSTALL_LIB_PATH}
        COMPONENT plugins)

