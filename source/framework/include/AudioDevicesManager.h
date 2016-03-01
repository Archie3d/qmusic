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
class MidiEventTranslator;

/**
 * @brief Single point of access to the audio devices.
 *
 * This class is used to centralize access to available audio devices.
 */
class QMUSIC_FRAMEWORK_API AudioDevicesManager : public QObject
{
    Q_OBJECT
public:

    AudioDevicesManager(QObject *pParent = nullptr);
    ~AudioDevicesManager();

    /**
     * Returns pointer to the current audio input device.
     * @return
     */
    AudioDevice* audioInputDevice() const { return m_pAudioInputDevice; }

    /**
     * Returns pointer to the current audio output device.
     * @return
     */
    AudioDevice* audioOutputDevice() const {return m_pAudioOutputDevice; }

    /**
     * Returns pointer to the current MIDI input device.
     * @return
     */
    MidiInputDevice* midiInputDevice() const { return m_pMidiInputDevice; }

    /**
     * Tells whether the audio devices have been started.
     * @return
     */
    bool isStarted() const { return m_started; }

public slots:

    /**
     * Start all audio devices.
     */
    void startAudioDevices();

    /**
     * Stop all audio devices.
     */
    void stopAudioDevices();

signals:

    /**
     * Signal used to notify that audio devices have not
     * been configured correctly.
     */
    void devicesNotConfigured();

private:

    /// Pointer to audio input device.
    AudioDevice *m_pAudioInputDevice;

    /// Pointer to audio output device;
    AudioDevice *m_pAudioOutputDevice;

    /// Pointer to MIDI input device;
    MidiInputDevice *m_pMidiInputDevice;

    /// Helper to translate MIDI messages to signal chain events
    MidiEventTranslator *m_pMidiEventTranslator;

    /// Audio devices started flag.
    bool m_started;
};

#endif // AUDIODEVICESMANAGER_H

