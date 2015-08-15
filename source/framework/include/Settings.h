/*
                          qmusic

    Copyright (C) 2015 Arthur Benilov,
    arthur.benilov@gmail.com

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.
*/

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
        Setting_MidiInChannel,  ///< MIDI input channel number.
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

