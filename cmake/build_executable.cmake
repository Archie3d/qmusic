cmake_policy(PUSH)
cmake_policy(SET CMP0020 OLD)
cmake_policy(SET CMP0043 OLD)

# Parse target source code
include(parse_src)

# Build an executable
add_executable(${PROJECT_NAME} ${SRC_CPP} ${SRC_C} ${SRC_H} ${SRC_H_PRIVATE} ${SRC_RES} ${WINRC})

if(${CMAKE_BUILD_TYPE} MATCHES "Release")
    if(NOT ${USE_CONSOLE})
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
        set_target_properties(${PROJECT_NAME} PROPERTIES RELWITHDEBINFO "/SUBSYSTEM:CONSOLE")
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
        set_target_properties(${PROJECT_NAME} PROPERTIES MINSIZEREL "/SUBSYSTEM:WINDOWS")

        if(USE_QT)
            target_link_libraries(${PROJECT_NAME} Qt5::WinMain)
        endif()
    endif()
endif()

# Link libraries
target_link_libraries(${PROJECT_NAME} ${DEPENDS})

# Link with dependencies
include(link_libs)

# Install this target
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${INSTALL_BIN_PATH} COMPONENT runtime)

cmake_policy(POP)
