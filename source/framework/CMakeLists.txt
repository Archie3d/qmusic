project(framework)

set(USE_QT TRUE)
set(DEPENDS_QT Widgets)
set(DEPENDS portaudio midi qtpropertybrowser)

if(APPLE)
    find_library(COREFOUNDATION CoreFoundation "/")
    find_library(COREAUDIO CoreAudio "/")
    find_library(COREMIDI CoreMidi "/")
endif()

include(build_shared_library)

if(APPLE)
    target_link_libraries(${PROJECT_NAME} ${COREFOUNDATION} ${COREAUDIO} ${COREMIDI})
endif()
