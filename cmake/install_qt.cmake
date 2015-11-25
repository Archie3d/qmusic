# Install Qt libraries

# Getting Qt version
find_package(Qt5Core)
string(REPLACE "." ";" QT_VERSION_LIST ${Qt5Core_VERSION_STRING})
list(GET QT_VERSION_LIST 0 QT_MAJOR_VERSION)
list(GET QT_VERSION_LIST 1 QT_MINOR_VERSION)
list(GET QT_VERSION_LIST 2 QT_PATCH_LEVEL)

set(QT_LIBS
    Qt5Core
    Qt5Gui
    Qt5OpenGL
    Qt5PrintSupport
    Qt5Svg
    Qt5Widgets
)

set(QT_PLATFORM_LIBS
    qminimal
    qoffscreen
    qwindows
)

set(ICU_VERSION "53")

if (QT_MINOR_VERSION EQUAL 5)
    set(ICU_VERSION "54")
endif()

set(ICU_LIBS
    icudt${ICU_VERSION}
    icuin${ICU_VERSION}
    icuuc${ICU_VERSION}
)

if(WIN32)

    # cmake does not like windows-like slash in path
    string(REPLACE "\\" "/" QT_DIR "${QT_DIR}")

    foreach(QT_LIB ${QT_LIBS})
        if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
            set(QT_LIB "${QT_LIB}d.dll")
        else()
            set(QT_LIB "${QT_LIB}.dll")
        endif()
        install(FILES "${QT_DIR}/bin/${QT_LIB}" DESTINATION ${INSTALL_BIN_PATH} COMPONENT runtime)
    endforeach()

    foreach(QT_PLATFORM_LIB ${QT_PLATFORM_LIBS})
        if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
            set(QT_PLATFORM_LIB "${QT_PLATFORM_LIB}d.dll")
        else()
            set(QT_PLATFORM_LIB "${QT_PLATFORM_LIB}.dll")
        endif()
        install(FILES "${QT_DIR}/plugins/platforms/${QT_PLATFORM_LIB}"
                DESTINATION "${INSTALL_BIN_PATH}/platforms" COMPONENT runtime)
    endforeach()

    foreach(ICU_LIB ${ICU_LIBS})
        set(ICU_LIB "${ICU_LIB}.dll")
        install(FILES "${QT_DIR}/bin/${ICU_LIB}" DESTINATION ${INSTALL_BIN_PATH} COMPONENT runtime)
    endforeach()

endif()
