project(portaudio)

set(DEPENDS asiosdk)

if(WIN32)
    set(HOST "win")
endif()

set(INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

file(GLOB COMMON_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/common/*.c*)
file(GLOB HOSTAPI_ASIO ${CMAKE_CURRENT_SOURCE_DIR}/src/hostapi/asio/*.c*)
file(GLOB HOSTAPI_WDMKS ${CMAKE_CURRENT_SOURCE_DIR}/src/hostapi/wdmks/*.c*)
file(GLOB HOSTAPI_SRC
    ${HOSTAPI_ASIO}
    ${HOSTAPI_WDMKS}
)
file(GLOB HOST_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/os/${HOST}/*.c*)

set(SRCS
    ${COMMON_SRC}
    ${HOSTAPI_SRC}
    ${HOST_SRC}
)

include_directories(
    ${INCLUDES}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hostapi/asio
    ${CMAKE_CURRENT_SOURCE_DIR}/src/os/${HOST}
)

# Dependencies includes
foreach(DEP ${DEPENDS})
    if(${DEP}_INCLUDES)
        include_directories(${${DEP}_INCLUDES})
    else()
        message(FATAL_ERROR "Library ${DEP} has not been processed yet. Required by target ${PROJECT_NAME}")
    endif()
endforeach()

set(${PROJECT_NAME}_INCLUDES ${INCLUDES} CACHE INTERNAL "${PROJECT_NAME} library includes" FORCE)

add_definitions(-DPA_USE_ASIO=1)
add_definitions(-DPA_USE_WDMKS=1)

# Disable compiler warnings
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions("/wd4996")
endif()

add_library(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} ${DEPENDS})
