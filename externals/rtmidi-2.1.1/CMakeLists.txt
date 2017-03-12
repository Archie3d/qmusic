project(rtmidi)

if(WIN32)
    add_definitions(-D__WINDOWS_MM__)
elseif(APPLE)
    add_definitions(-D__MACOSX_CORE__)
    add_definitions(-DHAVE_OSX_COREMIDI_)

    find_library(COREFOUNDATION CoreFoundation "/")
    find_library(COREAUDIO CoreAudio "/")
    find_library(COREMIDI CoreMidi "/")
elseif(UNIX)
    find_package(ALSA REQUIRED)
    add_definitions(-D__LINUX_ALSA__)
    include_directories(${ALSA_INCLUDE_DIR})

    #add_definitions(-D__UNIX_JACK__)
endif()

include(build_library)

if(APPLE)
    target_link_libraries(${PROJECT_NAME} ${COREFOUNDATION} ${COREAUDIO} ${COREMIDI})
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} ${ALSA_LIBRARY})

    #target_link_libraries(${PROJECT_NAME} jack)
endif()
