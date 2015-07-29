set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)
if(CMAKE_BUILD_TESTS AND EXISTS ${TEST_DIR})

    # Gather all unit tests
    file(GLOB TEST_HDRS ${TEST_DIR}/*.h)

    foreach(TEST_H ${TEST_HDRS})
        get_filename_component(FILENAME ${TEST_H} NAME_WE)

        # Generating test source file
        set(TEST_SRC ${CMAKE_CURRENT_BINARY_DIR}/test/test_${FILENAME}.cpp)
        file(WRITE ${TEST_SRC} "#include \"${TEST_H}\"\n")
        file(APPEND ${TEST_SRC} "QTEST_MAIN(${FILENAME})")

        # Test target
        set(TEST_TARGET "${PROJECT_NAME}_${FILENAME}")
        include_directories(${TEST_DIR})
        include_directories(${${PROJECT_NAME}_INCLUDES})
        add_executable(${TEST_TARGET} ${TEST_SRC} ${TEST_H})

        # Link with component under test and its dependencies
        target_link_libraries(${TEST_TARGET} ${PROJECT_NAME} ${DEPENDS})

        # Link with Qt framework
        qt5_use_modules(${TEST_TARGET} Core Test ${DEPENDS_QT})

        # Add test to cmake
        add_test(${TEST_TARGET} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_TARGET})
    endforeach()
endif()
