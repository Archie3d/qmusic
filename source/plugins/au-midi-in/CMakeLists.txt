project(au-midi-in)

set(USE_QT TRUE)
set(DEPENDS_QT Widgets)

set(DEPENDS framework midi qtpropertybrowser)

include(build_plugin)
