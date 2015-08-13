project(qwt)

set(USE_QT TRUE)
set(DEPENDS_QT Widgets OpenGL Concurrent PrintSupport Svg)

include(build_shared_library)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-DQWT_DLL)
add_definitions(-DQWT_MAKEDLL)
if(WIN32)
    # Disble warning C4267: '=' : conversion from 'size_t' to 'int', possible loss of data 
    add_definitions(/wd4267)
endif()
