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
#include <QMap>
#include "portaudio.h"
#include "Application.h"
#include "AudioDevice.h"

static int audioDeviceCallback(const void *pInputBuffer,
                               void *pOutputBuffer,
                               unsigned long framesPerBuffer,
                               const PaStreamCallbackTimeInfo *pTimeInfo,
                                PaStreamCallbackFlags statusFlags,
                                void *pData)
{
    AudioDevice *pAudioDevice = static_cast<AudioDevice*>(pData);

    const float* pIn = static_cast<const float*>(pInputBuffer);
    float *pOut = static_cast<float*>(pOutputBuffer);

    pAudioDevice->processAudio(pIn, pOut, framesPerBuffer);

    return 0;
}


AudioDevice::AudioDevice()
    : m_pStream(nullptr)
{
    int err = Pa_Initialize();
    if (err != paNoError) {
        qCritical() << "Unable initialize audio device:" << Pa_GetErrorText(err);
    }

    m_openDeviceInfo.index = paNoDevice;
    m_openDeviceInfo.name = "No Device";
    m_openDeviceInfo.nInputs = 0;
    m_openDeviceInfo.nOutputs = 0;
    m_openDeviceInfo.sampleRate = 0.0f;
}

AudioDevice::~AudioDevice()
{
    Pa_Terminate();
}

QList<AudioDevice::Info> AudioDevice::enumarate() const
{
    QList<Info> devices;

    PaDeviceIndex n = Pa_GetDeviceCount();
    for (PaDeviceIndex i = 0; i < n; i++) {
        devices.append(getInfo(i));
    }

    return devices;
}

bool AudioDevice::open(int index, int nInputs, int nOutputs, double sampleRate, int bufferSize)
{
    if (isOpen()) {
        if (!close()) {
            return false;
        }
    }

    PaStreamParameters inputParameters;
    inputParameters.device = index;
    inputParameters.channelCount = nInputs;
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(index)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    PaStreamParameters *pInputParamaters = nInputs > 0 ? &inputParameters : NULL;

    PaStreamParameters outputParameters;
    outputParameters.device = index;
    outputParameters.channelCount = nOutputs;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(index)->defaultLowInputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    PaStreamParameters *pOutputParameters = nOutputs > 0 ? &outputParameters : NULL;

    m_openDeviceInfo = getInfo(index);
    m_openDeviceInfo.nInputs = nInputs;
    m_openDeviceInfo.nOutputs = nOutputs;
    m_openDeviceInfo.sampleRate = sampleRate;

    int err = Pa_OpenStream(&m_pStream,
                            pInputParamaters,
                            pOutputParameters,
                            sampleRate,
                            bufferSize,
                            0, // paClipOff
                            audioDeviceCallback,
                            this
                           );

    if (err != paNoError) {
        qCritical() << "Unable to open audio device (" << m_openDeviceInfo.name << ")"
                    << Pa_GetErrorText(err)
                    << "Error code" << Pa_GetLastHostErrorInfo()->errorCode;
        return false;
    }


    return true;
}

bool AudioDevice::close()
{
    if (!isOpen()) {
        return true;
    }

    int err = Pa_CloseStream(m_pStream);
    if (err != paNoError) {
        qCritical() << "Unable to close audio device (" << m_openDeviceInfo.name << ")"
                    << Pa_GetErrorText(err)
                    << "Error code" << Pa_GetLastHostErrorInfo()->errorCode;
        return false;
    }

    m_pStream = nullptr;
    return true;
}

bool AudioDevice::isOpen() const
{
    return m_pStream != nullptr;
}

bool AudioDevice::start()
{
    if (!isOpen()) {
        qCritical() << "Unable to start audio stream: device is closed";
        return false;
    }

    int err = Pa_StartStream(m_pStream);
    if (err != paNoError) {
        qCritical() << "Unable to start audio stream:" << Pa_GetErrorText(err)
                    << "Error code:" << Pa_GetLastHostErrorInfo()->errorCode;
        return false;
    }

    return true;
}

bool AudioDevice::stop()
{
    if (!isOpen()) {
        return true;
    }

    int err = Pa_StopStream(m_pStream);
    if (err != paNoError) {
        qCritical() << "Unable to stop audio stream:" << Pa_GetErrorText(err)
                    << "Error code:" << Pa_GetLastHostErrorInfo()->errorCode;
        return false;
    }

    return true;
}

void AudioDevice::addListener(IAudioDeviceListener *pListener)
{
    Q_ASSERT(pListener != nullptr);
    m_listeners.append(pListener);
}

void AudioDevice::removeListener(IAudioDeviceListener *pListener)
{
    m_listeners.removeAll(pListener);
}

void AudioDevice::processAudio(const float *pInputBuffer, float *pOutputBuffer, long nSamples)
{
    for (IAudioDeviceListener *pListener : m_listeners) {
        pListener->processAudio(pInputBuffer, pOutputBuffer, nSamples);
    }
}

AudioDevice::Info AudioDevice::getInfo(int index) const
{
    Info devInfo;
    const PaDeviceInfo *pInfo = Pa_GetDeviceInfo(index);
    const PaHostApiInfo *pHostApiInfo = Pa_GetHostApiInfo(pInfo->hostApi);

    devInfo.index = index;
    devInfo.name = QString::fromLatin1(pInfo->name);
    devInfo.hostApi = QString::fromLatin1(pHostApiInfo->name);
    devInfo.nInputs = pInfo->maxInputChannels;
    devInfo.nOutputs = pInfo->maxOutputChannels;
    devInfo.sampleRate = pInfo->defaultSampleRate;
    return devInfo;
}
