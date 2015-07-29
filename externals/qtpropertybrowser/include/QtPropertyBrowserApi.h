
#ifndef QTPROPERTYBROWSERAPI_H
#define QTPROPERTYBROWSERAPI_H

#if defined(_WIN32)
#   if defined(qtpropertybrowser_EXPORTS)
#       define QT_QTPROPERTYBROWSER_EXPORT __declspec(dllexport)
#   else
#       define QT_QTPROPERTYBROWSER_EXPORT __declspec(dllimport)
#   endif
#else
#   define QT_QTPROPERTYBROWSER_EXPORT
#endif

#endif // QTPROPERTYBROWSERAPI_H
