project(au-mixer)

set(USE_QT TRUE)
set(DEPENDS_QT Widgets)

set(DEPENDS portaudio framework qtpropertybrowser)

include(build_plugin)
