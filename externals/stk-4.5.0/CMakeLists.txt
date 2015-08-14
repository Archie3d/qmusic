#
#   This library is a subset of the STK code
#

project(stk)

set(USE_QT FALSE)

include(build_library)

add_definitions(-D__LITTLE_ENDIAN__)

if(WIN32)
    add_definitions(-D__WINDOWS_MM__)

    # Disable STK compilation warnings
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(/wd4244)
    add_definitions(/wd4267)
endif()

set(STK_RAWWAVES "${CMAKE_CURRENT_SOURCE_DIR}/rawwaves")
file(COPY ${STK_RAWWAVES} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
install(DIRECTORY ${STK_RAWWAVES} DESTINATION ${INSTALL_BIN_PATH} COMPONENT runtime)
