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
#include "AudioDevice.h"
#include "MidiInputDevice.h"
#include "AudioDevicesManager.h"

const int cNumberOfChannels(2);

AudioDevicesManager::AudioDevicesManager(QObject *pParent)
    : QObject(pParent)
{
    m_pAudioInputDevice = new AudioDevice();
    m_pAudioOutputDevice = new AudioDevice();
    m_pMidiInputDevice = new MidiInputDevice();
}

AudioDevicesManager::~AudioDevicesManager()
{
    delete m_pAudioInputDevice;
    delete m_pAudioOutputDevice;
    delete m_pMidiInputDevice;
}

void AudioDevicesManager::startAudioDevices()
{
    Settings settings;

    Q_ASSERT(m_pAudioInputDevice != nullptr);
    Q_ASSERT(m_pAudioOutputDevice != nullptr);
    Q_ASSERT(m_pMidiInputDevice != nullptr);

    int waveInDeviceIndex = settings.get(Settings::Setting_WaveInIndex).toInt();
    int waveOutDeviceIndex = settings.get(Settings::Setting_WaveOutIndex).toInt();
    int midiInDeviceIndex = settings.get(Settings::Setting_MidiInIndex).toInt();
    int midiInChannel = settings.get(Settings::Setting_MidiInChannel).toInt();

    double sampleRate = settings.get(Settings::Setting_SampleRate).toDouble();
    int bufferSize = settings.get(Settings::Setting_BufferSize).toInt();

    //setTimeStep(1.0 / sampleRate);

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

    if (midiInDeviceIndex >= 0) {
        m_pMidiInputDevice->setNumber(midiInDeviceIndex);
        m_pMidiInputDevice->setChannel(midiInChannel);
        if (m_pMidiInputDevice->open()) {
            m_pMidiInputDevice->start();
        } else {
            qCritical() << "Failed to open MIDI input device";
        }
    }
}

void AudioDevicesManager::stopAudioDevices()
{
    Q_ASSERT(m_pAudioInputDevice != nullptr);
    Q_ASSERT(m_pAudioOutputDevice != nullptr);
    Q_ASSERT(m_pMidiInputDevice != nullptr);

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
}
