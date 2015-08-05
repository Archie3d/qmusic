#ifndef QMUSIC_DSPAPI_H
#define QMUSIC_DSPAPI_H

#if defined(_WIN32)
#   if defined(dsp_EXPORTS)
#       define QMUSIC_DSP_API __declspec(dllexport)
#   else
#       define QMUSIC_DSP_API __declspec(dllimport)
#   endif
#else
#   define QMUSIC_DSP_API
#endif

#endif // QMUSIC_DSP_H
