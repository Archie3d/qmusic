project(au-math-expression)

set(USE_QT TRUE)
set(DEPENDS_QT Widgets)

set(DEPENDS framework qtpropertybrowser)

if(WIN32)
    # Disble warning C4244: 'argument' : conversion from 'std::iterator_traits<const char *>::difference_type' to 'const int', possible loss of data
    add_definitions(/wd4244)

    # fatal error C1128: number of sections exceeded object file format limit : compile with /bigobj
    add_definitions(/bigobj)
endif()

include(build_plugin)
