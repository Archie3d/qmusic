project(au-stk-bowed)

set(USE_QT TRUE)
set(DEPENDS_QT Widgets)

set(DEPENDS framework stk qtpropertybrowser)

include(build_plugin)

if(WIN32)
    # Disable STK compilation warnings
    add_definitions(/wd4267)
endif()
