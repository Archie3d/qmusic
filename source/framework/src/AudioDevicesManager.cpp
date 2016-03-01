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

#include <QDebug>
#include "Settings.h"
#include "Application.h"
#include "AudioDevice.h"
#include "MidiInputDevice.h"
#include "IEventRouter.h"
#include "ISignalChain.h"
#include "SignalChainEvent.h"
#include "NoteOnEvent.h"
#include "NoteOffEvent.h"
#include "PitchBendEvent.h"
#include "ControllerEvent.h"
#include "AudioDevicesManager.h"

const int cNumberOfChannels(2);

class MidiEventTranslator : public IMidiInputListener
{
public:
    MidiEventTranslator()
    {
        m_running = false;
    }

    void start()
    {
        m_running = true;
    }

    void stop()
    {
        m_running = false;
    }

    void inputMidiMessage(const MidiMessage &msg)
    {
        if (!m_running) {
            return;
        }

        SignalChainEvent *pEvent = nullptr;

        switch (msg.status()) {
        case MidiMessage::Status_NoteOn:
            if (msg.velocity() == 0) {
                pEvent = new NoteOffEvent(msg.noteNumber(), 64);
            } else {
                pEvent = new NoteOnEvent(msg.noteNumber(), msg.velocity());
            }
            break;
        case MidiMessage::Status_NoteOff:
            pEvent = new NoteOffEvent(msg.noteNumber(), msg.velocity());
            break;
        case MidiMessage::Status_PitchBend: {
            pEvent = new PitchBendEvent(msg.pitchBend());
            break;
        }
        case MidiMessage::Status_ControlChange: {
            pEvent = new ControllerEvent(msg.controllerNumber(), msg.controllerValue());
            break;
        }
        default:
            break;
        }

        if (pEvent != nullptr) {
            // Route MIDI event via the global router
            Application::instance()->eventRouter()->postEvent(pEvent);
        }
    }

private:

    bool m_running;

};

/*
 *  AudioDeviceManager implementation
 */

AudioDevicesManager::AudioDevicesManager(QObject *pParent)
    : QObject(pParent)
{
    m_pAudioInputDevice = new AudioDevice();
    m_pAudioOutputDevice = new AudioDevice();
    m_pMidiInputDevice = new MidiInputDevice();

    m_pMidiEventTranslator = new MidiEventTranslator();
    m_pMidiInputDevice->addListener(m_pMidiEventTranslator);
    m_started = false;
}

AudioDevicesManager::~AudioDevicesManager()
{
    delete m_pAudioInputDevice;
    delete m_pAudioOutputDevice;
    delete m_pMidiInputDevice;
    delete m_pMidiEventTranslator;
}

void AudioDevicesManager::startAudioDevices()
{
    Settings settings;

    Q_ASSERT(m_pAudioInputDevice != nullptr);
    Q_ASSERT(m_pAudioOutputDevice != nullptr);
    Q_ASSERT(m_pMidiInputDevice != nullptr);

    int waveInDeviceIndex = settings.get(Settings::Setting_WaveInIndex).toInt();
    int waveOutDeviceIndex = settings.get(Settings::Setting_WaveOutIndex).toInt();

    if (waveInDeviceIndex < 0 || waveOutDeviceIndex < 0) {
        // Wave devices have not been configured
        qWarning() << "Audio interfaces have not been configured yet";
        emit devicesNotConfigured();
        return;
    }

    int midiInDeviceIndex = settings.get(Settings::Setting_MidiInIndex).toInt();
    int midiInChannel = settings.get(Settings::Setting_MidiInChannel).toInt();

    double sampleRate = settings.get(Settings::Setting_SampleRate).toDouble();
    int bufferSize = settings.get(Settings::Setting_BufferSize).toInt();

    if (waveInDeviceIndex == waveOutDeviceIndex) {
        // Open only one device
        if (m_pAudioOutputDevice->open(waveOutDeviceIndex, cNumberOfChannels, cNumberOfChannels, sampleRate, bufferSize)) {
            m_pAudioOutputDevice->start();
        }
    } else {
        // Different devices for input and output
        if (m_pAudioInputDevice->open(waveInDeviceIndex, cNumberOfChannels, 0, sampleRate, bufferSize)) {
            m_pAudioInputDevice->start();
        }
        if (m_pAudioOutputDevice->open(waveOutDeviceIndex, 0, cNumberOfChannels, sampleRate, bufferSize)) {
            m_pAudioOutputDevice->start();
        }
    }

    // Make sure to clear all events
    Application::instance()->eventRouter()->purge();
    m_pMidiEventTranslator->start();

    if (midiInDeviceIndex >= 0) {
        m_pMidiInputDevice->setNumber(midiInDeviceIndex);
        m_pMidiInputDevice->setChannel(midiInChannel);
        if (m_pMidiInputDevice->open()) {
            m_pMidiInputDevice->start();
        } else {
            qCritical() << "Failed to open MIDI input device";
        }
    }

    m_started = true;
}

void AudioDevicesManager::stopAudioDevices()
{
    Q_ASSERT(m_pAudioInputDevice != nullptr);
    Q_ASSERT(m_pAudioOutputDevice != nullptr);
    Q_ASSERT(m_pMidiInputDevice != nullptr);

    m_pMidiEventTranslator->stop();

    if (m_pAudioInputDevice->isOpen()) {
        m_pAudioInputDevice->stop();
        m_pAudioInputDevice->close();
    }

    if (m_pAudioOutputDevice->isOpen()) {
        m_pAudioOutputDevice->stop();
        m_pAudioOutputDevice->close();
    }

    if (m_pMidiInputDevice->isOpen()) {
        m_pMidiInputDevice->stop();
        m_pMidiInputDevice->close();
    }

    // Purge unhandled events
    Application::instance()->eventRouter()->purge();

    m_started = false;
}
