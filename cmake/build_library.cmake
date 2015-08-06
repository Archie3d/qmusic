# Parse target source code
include(parse_src)

# Cache library includes folder
set(${PROJECT_NAME}_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE INTERNAL "${PROJECT_NAME} library includes" FORCE)

# Build static library
add_library(${PROJECT_NAME} ${SRC_CPP} ${SRC_C} ${SRC_H} ${SRC_H_PRIVATE})
target_link_libraries(${PROJECT_NAME} ${DEPENDS})

# Link with dependencies
include(link_libs)

# Process unit tests
include(parse_tests)
