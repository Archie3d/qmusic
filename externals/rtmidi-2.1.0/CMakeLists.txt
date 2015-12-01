project(rtmidi)

if(WIN32)
    add_definitions(-D__WINDOWS_MM__)
elseif(APPLE)
    add_definitions(-D__MACOSX_CORE__)
    add_definitions(-DHAVE_OSX_COREMIDI_)

    find_library(COREFOUNDATION CoreFoundation "/")
    find_library(COREAUDIO CoreAudio "/")
    find_library(COREMIDI CoreMidi "/")
endif()

include(build_library)

if(APPLE)
    target_link_libraries(${PROJECT_NAME} ${COREFOUNDATION} ${COREAUDIO} ${COREMIDI})
endif()
