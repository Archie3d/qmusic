project(qmusic)

set(USE_QT TRUE)
set(USE_CONSOLE FALSE)
set(DEPENDS_QT Widgets)

set(DEPENDS framework view)

include(build_executable)
