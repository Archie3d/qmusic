project(portaudio)

if(WIN32)
    set(HOST "win")
    set(DEPENDS asiosdk)
else()
    set(HOST "unix")
endif()

set(INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

file(GLOB COMMON_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/common/*.c*)
file(GLOB HOSTAPI_ASIO ${CMAKE_CURRENT_SOURCE_DIR}/src/hostapi/asio/*.c*)
file(GLOB HOSTAPI_WDMKS ${CMAKE_CURRENT_SOURCE_DIR}/src/hostapi/wdmks/*.c*)
file(GLOB HOSTAPI_WASAPI ${CMAKE_CURRENT_SOURCE_DIR}/src/hostapi/wasapi/*.c*)
file(GLOB HOSTAPI_WMME ${CMAKE_CURRENT_SOURCE_DIR}/src/hostapi/wmme/*.c*)
file(GLOB HOSTAPI_COREAUDIO ${CMAKE_CURRENT_SOURCE_DIR}/src/hostapi/coreaudio/*.c*)


if(WIN32)
    file(GLOB HOSTAPI_SRC
        ${HOSTAPI_ASIO}
        ${HOSTAPI_WDMKS}
        ${HOSTAPI_WASAPI}
        ${HOSTAPI_WMME}
    )
elseif(APPLE)
    file(GLOB HOSTAPI_SRC
        ${HOSTAPI_COREAUDIO}
    )
endif()

file(GLOB HOST_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/os/${HOST}/*.c*)

set(SRCS
    ${COMMON_SRC}
    ${HOSTAPI_SRC}
    ${HOST_SRC}
)

include_directories(
    ${INCLUDES}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common
    ${CMAKE_CURRENT_SOURCE_DIR}/src/os/${HOST}
)

if(WIN32)
    list(APPEND INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/src/hostapi/asio)
endif()

# Dependencies includes
foreach(DEP ${DEPENDS})
    if(${DEP}_INCLUDES)
        include_directories(${${DEP}_INCLUDES})
    else()
        message(FATAL_ERROR "Library ${DEP} has not been processed yet. Required by target ${PROJECT_NAME}")
    endif()
endforeach()

set(${PROJECT_NAME}_INCLUDES ${INCLUDES} CACHE INTERNAL "${PROJECT_NAME} library includes" FORCE)

if(WIN32)
    add_definitions(-DPA_USE_ASIO=1)
    add_definitions(-DPA_USE_WDMKS=1)
    add_definitions(-DPA_USE_WASAPI=1)
    add_definitions(-DPA_USE_WMME=1)
elseif(APPLE)
    add_definitions(-DPA_USE_COREAUDIO=1)

    find_library(COREFOUNDATION CoreFoundation "/")
    find_library(CORESERVICES CoreServices "/")
    find_library(COREAUDIO CoreAudio "/")
    find_library(AUDIOUNIT AudioUnit "/")
    find_library(AUDIOTOOLBOX AudioToolbox "/")
    find_library(COREMIDI CoreMidi "/")
endif()

# Disable compiler warnings
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions("/wd4996")
endif()

add_library(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} ${DEPENDS})

if(APPLE)
    target_link_libraries(${PROJECT_NAME} ${COREFOUNDATION} ${CORESERVICES} ${COREAUDIO} ${AUDIOUNIT} ${AUDIOTOOLBOX} ${COREMIDI})
endif()
