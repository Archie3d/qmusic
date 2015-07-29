project(asiosdk)

set(INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/host
)

set(SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/common/asio.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/host/asiodrivers.cpp
)

if(WIN32)
    list(APPEND INCLUDES
         ${CMAKE_CURRENT_SOURCE_DIR}/host/pc
    )

    list(APPEND SRCS
         ${CMAKE_CURRENT_SOURCE_DIR}/host/pc/asiolist.cpp
    )
endif()

include_directories(${INCLUDES})

set(${PROJECT_NAME}_INCLUDES ${INCLUDES} CACHE INTERNAL "${PROJECT_NAME} library includes" FORCE)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
if(WIN32)
    # Disble warning C4267: 'argument' : conversion from 'size_t' to 'DWORD', possible loss of data
    add_definitions(/wd4267)
endif()

add_library(${PROJECT_NAME} ${SRCS})
