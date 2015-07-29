#ifndef QMUSIC_FRAMEWORKAPI_H
#define QMUSIC_FRAMEWORKAPI_H

#if defined(_WIN32)
#   if defined(framework_EXPORTS)
#       define QMUSIC_FRAMEWORK_API __declspec(dllexport)
#   else
#       define QMUSIC_FRAMEWORK_API __declspec(dllimport)
#   endif
#else
#   define QMUSIC_FRAMEWORK_API
#endif

#endif // QMUSIC_FRAMEWORKAPI_H
