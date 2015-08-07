project(au-midi-in-ctrl)

set(USE_QT TRUE)
set(DEPENDS_QT Widgets)

set(DEPENDS framework midi qtpropertybrowser)

include(build_plugin)
