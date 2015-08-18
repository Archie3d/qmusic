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

#include <chrono>
#include <QTimer>
#include <QVector>
#include "ISignalChain.h"
#include "AudioBuffer.h"
#include "SpeakerThreadObject.h"

SpeakerThreadObject::SpeakerThreadObject(long bufferSize, QObject *pParent)
    : QObject(pParent),
      m_bufferSize(bufferSize)
{
    m_pSignalChain = nullptr;
    m_pLeftBuffer = new AudioBuffer(bufferSize * 2);
    m_pRightBuffer = new AudioBuffer(bufferSize * 2);
    m_pLeftData = new float[bufferSize * 2];
    m_pRightData = new float[bufferSize * 2];
    m_pSignalBuffer = nullptr;
    m_signalIndex = 0;
    m_singnalUpdateRequested = false;

    m_started = false;
    m_dspLoad = 0.0f;

    connect(this, SIGNAL(started()), this, SLOT(generateSamples()), Qt::QueuedConnection);
    connect(this, SIGNAL(continueGenerateSamples()),
            this, SLOT(generateSamples()), Qt::QueuedConnection);
}

SpeakerThreadObject::~SpeakerThreadObject()
{
    delete m_pLeftBuffer;
    delete m_pRightBuffer;
    delete[] m_pLeftData;
    delete[] m_pRightData;
}

void SpeakerThreadObject::setSignalChain(ISignalChain *pSignalChain)
{
    Q_ASSERT(pSignalChain != nullptr);
    QMutexLocker lock(&m_mutex);
    m_pSignalChain = pSignalChain;
}

void SpeakerThreadObject::setInputPorts(InputPort *pLeft, InputPort *pRight)
{
    QMutexLocker lock(&m_mutex);
    m_pLeftChannelInput = pLeft;
    m_pRightChannelInput = pRight;
}

void SpeakerThreadObject::start()
{
    Q_ASSERT(m_pSignalBuffer != nullptr);

    QMutexLocker lock(&m_mutex);
    m_started = true;
    m_firstBuffer = true;
    m_pLeftBuffer->clear();
    m_pRightBuffer->clear();
    m_dspLoad = 0.0;
    setDspLoad(0.0f);
    m_signalIndex = 0;
    m_pSignalBuffer->fill(0.0f);

    emit started();
}

void SpeakerThreadObject::stop()
{
    QMutexLocker lock(&m_mutex);
    m_started = false;
    setDspLoad(0.0f);
    m_signalIndex = 0;
}

void SpeakerThreadObject::signalUpdateOver()
{
    m_signalIndex = 0;
    m_singnalUpdateRequested = false;
}

float SpeakerThreadObject::getNextLeftChannelSample()
{
    m_pLeftChannelInput->update();
    return m_pLeftChannelInput->value().asFloat;
}

float SpeakerThreadObject::getNextRightChannelSample()
{
    m_pRightChannelInput->update();
    return m_pRightChannelInput->value().asFloat;
}

void SpeakerThreadObject::generateSamples()
{
    // This method is always called from this object's thread
    if (!m_started) {
        return;
    }

    long available = m_pLeftBuffer->availableToWrite();
    available = qMin(available, m_pRightBuffer->availableToWrite());

    if (available < m_bufferSize / 2) {
        // If availability is low, trigger timer        
        //emit continueGenerateSamples(); // This will behave as busy waiting with 100% CPU code load

        // Process pending events
        m_pSignalChain->processEvents();

        // Add some sleep time waiting for buffer
        QTimer::singleShot(500 * m_pSignalChain->timeStep() * m_bufferSize, this, SLOT(generateSamples()));
        return;
    } else {

        // NOTE:
        // On Windows 7 the time measurement floats considerably if the process
        // does not receive system messages frequently.
        // This can be fixed by flooding the event loop (emit continueGenerateSamples as shown above).
        // This will however increates the CPU core usage to 100%.
        //
        // Windows 8 seems to provide accurate time measurements.

        double realTimeUs = m_pSignalChain->timeStep() * available * 1.0e6;

        auto startTime = std::chrono::high_resolution_clock::now();
        for (long i = 0; i < available; i++) {
            m_pSignalChain->prepareUpdate();
            m_pLeftData[i] = getNextLeftChannelSample();
            m_pRightData[i] = getNextRightChannelSample();
            if (m_signalIndex < m_pSignalBuffer->size()) {
                float s = 0.5 * (m_pLeftData[i] + m_pRightData[i]);
                (*m_pSignalBuffer)[m_signalIndex++] = s;
            }
        }
        m_pLeftBuffer->write(m_pLeftData, available);
        m_pRightBuffer->write(m_pRightData, available);

        // Estimate DSP load as ratio of processing time vs real synthesis time.
        auto processingTime = std::chrono::high_resolution_clock::now() - startTime;
        double dspTimeUs = double(std::chrono::duration_cast<std::chrono::microseconds>(processingTime).count());
        setDspLoad(dspTimeUs / realTimeUs);

        if (m_signalIndex >= m_pSignalBuffer->size() && !isSignalUpdateRequested()) {
            requestSignalUpdate();
        }

        // Continue with the samples generation
        emit continueGenerateSamples();
    }

    if (m_firstBuffer) {
        m_firstBuffer = false;
        emit bufferReady();
    }
}

void SpeakerThreadObject::setDspLoad(float l)
{
    float dl = fabs(m_dspLoad - l);
    if (dl >= 0.05) {
        // Do some averaging to filter rapid changes
        m_dspLoad = m_dspLoad * 0.9 + l * 0.1;
        emit dspLoadChanged(m_dspLoad);
    }
}

void SpeakerThreadObject::requestSignalUpdate()
{
    emit signalChanged();
}
