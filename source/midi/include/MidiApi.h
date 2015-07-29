#ifndef QMUSIC_MIDIAPI_H
#define QMUSIC_MIDIAPI_H

#if defined(_WIN32)
#   if defined(midi_EXPORTS)
#       define QMUSIC_MIDI_API __declspec(dllexport)
#   else
#       define QMUSIC_MIDI_API __declspec(dllimport)
#   endif
#else
#   define QMUSIC_MIDI_API
#endif

#endif // QMUSIC_MIDIAPI_H
