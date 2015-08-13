project(view)

set(USE_QT TRUE)
set(DEPENDS_QT Widgets)
set(DEPENDS framework midi dsp qtpropertybrowser qwt)

include(build_shared_library)
