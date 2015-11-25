project(rtmidi)

if(WIN32)
    add_definitions(-D__WINDOWS_MM__=1)
endif()

include(build_library)

