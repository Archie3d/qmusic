#ifndef QMUSIC_VIEWAPI_H
#define QMUSIC_VIEWAPI_H

#if defined(_WIN32)
#   if defined(view_EXPORTS)
#       define QMUSIC_VIEW_API __declspec(dllexport)
#   else
#       define QMUSIC_VIEW_API __declspec(dllimport)
#   endif
#else
#   define QMUSIC_VIEW_API
#endif

#endif // QMUSIC_VIEWAPI_H
