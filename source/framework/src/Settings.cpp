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

#include <QMap>
#include <QSettings>
#include "Application.h"
#include "Settings.h"

// Settings default values
const QMap<Settings::Setting, QVariant> cDefaultSettings = []() {
    QMap<Settings::Setting, QVariant> map;
    map[Settings::Setting_WaveInIndex] = -1;
    map[Settings::Setting_WaveOutIndex] = -1;
    map[Settings::Setting_MidiInIndex] = -1;
    map[Settings::Setting_MidiInChannel] = 1;
    map[Settings::Setting_MidiOutIndex] = -1;
    map[Settings::Setting_SampleRate] = 44100.0;
    map[Settings::Setting_BufferSize] = 1024;
    return map;
}();

const QMap<Settings::Setting, QString> cSettingsNameMap = []() {
    QMap<Settings::Setting, QString> map;
    map[Settings::Setting_WaveInIndex] = "waveInIndex";
    map[Settings::Setting_WaveOutIndex] = "waveOutIndex";
    map[Settings::Setting_MidiInIndex] = "midiInIndex";
    map[Settings::Setting_MidiInChannel] = "midiInChannel";
    map[Settings::Setting_MidiOutIndex] = "midiOutIndex";
    map[Settings::Setting_SampleRate] = "sampleRate";
    map[Settings::Setting_BufferSize] = "bufferSize";
    return map;
}();

Settings::Settings()
    : m_settings(Application::Company, Application::Product)
{
}

QVariant Settings::get(Settings::Setting s) const
{
    return m_settings.value(cSettingsNameMap[s], cDefaultSettings[s]);
}

void Settings::set(Settings::Setting s, const QVariant &value)
{
    m_settings.setValue(cSettingsNameMap[s], value);
}
