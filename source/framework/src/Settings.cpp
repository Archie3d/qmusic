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
const QMap<Settings::Setting, QVariant> cDefaultSettings {
    {Settings::Setting_WaveInIndex, -1},
    {Settings::Setting_WaveOutIndex, -1},
    {Settings::Setting_MidiInIndex, -1},
    {Settings::Setting_MidiInChannel, 1},
    {Settings::Setting_MidiOutIndex, -1},
    {Settings::Setting_SampleRate, 44100.0},
    {Settings::Setting_BufferSize, 1024}
};

const QMap<Settings::Setting, QString> cSettingsNameMap {
    {Settings::Setting_WaveInIndex, "waveInIndex"},
    {Settings::Setting_WaveOutIndex, "waveOutIndex"},
    {Settings::Setting_MidiInIndex, "midiInIndex"},
    {Settings::Setting_MidiInChannel, "midiInChannel"},
    {Settings::Setting_MidiOutIndex, "midiOutIndex"},
    {Settings::Setting_SampleRate, "sampleRate"},
    {Settings::Setting_BufferSize, "bufferSize"}
};

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
