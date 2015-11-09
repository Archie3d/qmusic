project(au-dial)

set(USE_QT TRUE)
set(DEPENDS_QT Widgets)

set(DEPENDS portaudio framework qtpropertybrowser)

include(build_plugin)
