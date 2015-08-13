project(au-speaker)

set(USE_QT TRUE)
set(DEPENDS_QT Widgets)

set(DEPENDS portaudio framework view qtpropertybrowser)

include(build_plugin)
