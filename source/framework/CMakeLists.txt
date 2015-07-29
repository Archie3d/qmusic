project(framework)

set(USE_QT TRUE)
set(DEPENDS_QT Widgets)
set(DEPENDS portaudio qtpropertybrowser)

include(build_shared_library)
