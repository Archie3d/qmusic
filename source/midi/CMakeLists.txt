project(midi)

set(USE_QT TRUE)
set(DEPENDS_QT Core)

set(DEPENDS rtmidi)

include(build_shared_library)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} Winmm)
endif()
