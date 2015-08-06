#ifndef SETTINGS_H
#define SETTINGS_H

#include <QVariant>
#include <QSettings>
#include "FrameworkApi.h"

/**
 * @brief Global application settings
 *
 * This class is used to access global application settings.
 */
class QMUSIC_FRAMEWORK_API Settings
{
public:

    /**
     * Enumeration of available settings.
     */
    enum Setting {
        Setting_WaveInIndex,    ///< Index of wave input device.
        Setting_WaveOutIndex,   ///< Index of wave output device.
        Setting_MidiInIndex,    ///< Index of MIDI input device.
        Setting_MidiOutIndex,   ///< Index of MIDI output device.
        Setting_SampleRate,     ///< Processing sample rate.
        Setting_BufferSize      ///< Audio buffer size.
    };

    Settings();

    /**
     * Returns setting's value. If setting has never been set, the default
     * value will be returned.
     * @param s Setting.
     * @return Setting value.
     */
    QVariant get(Settings::Setting s) const;

    /**
     * Assign setting value.
     * @param s Setting.
     * @param value Value to be assigned.
     */
    void set(Settings::Setting s, const QVariant &value);

private:

    /// System settings.
    QSettings m_settings;

};

#endif // SETTINGS_H

