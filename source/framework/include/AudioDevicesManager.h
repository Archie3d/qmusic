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

#ifndef AUDIODEVICESMANAGER_H
#define AUDIODEVICESMANAGER_H

#include <QObject>
#include "FrameworkApi.h"

class AudioDevice;
class MidiInputDevice;

/**
 * Single point of access to the audio devices.
 */
class QMUSIC_FRAMEWORK_API AudioDevicesManager : public QObject
{
    Q_OBJECT
public:

    AudioDevicesManager(QObject *pParent = nullptr);
    ~AudioDevicesManager();

    AudioDevice* audioInputDevice() const { return m_pAudioInputDevice; }
    AudioDevice* audioOutputDevice() const {return m_pAudioOutputDevice; }
    MidiInputDevice* midiInputDevice() const { return m_pMidiInputDevice; }

public slots:

    void startAudioDevices();
    void stopAudioDevices();

private:

    /// Pointer to audio input device.
    AudioDevice *m_pAudioInputDevice;

    /// Pointer to audio output device;
    AudioDevice *m_pAudioOutputDevice;

    /// Pointer to MIDI input device;
    MidiInputDevice *m_pMidiInputDevice;
};

#endif // AUDIODEVICESMANAGER_H

