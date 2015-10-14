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

#ifndef SPEAKERTHREADOBJECT_H
#define SPEAKERTHREADOBJECT_H

#include <QObject>
#include <QMutex>
#include <QVector>
#include "InputPort.h"

class ISignalChain;
class AudioUnit;
class AudioBuffer;

/**
 * This object is used to trigger the signal chain and it should reside in a
 * seperate thread.
 */
class SpeakerThreadObject : public QObject
{
    Q_OBJECT
public:

    SpeakerThreadObject(long bufferSize, QObject *pParent = nullptr);
    ~SpeakerThreadObject();

    void setSignalBuffer(QVector<float> *pBuffer) { m_pSignalBuffer = pBuffer; }

    AudioBuffer* leftChannelBuffer() const { return m_pLeftBuffer; }
    AudioBuffer* rightChannelBuffer() const { return m_pRightBuffer; }

    void setSignalChain(ISignalChain *pSignalChain);
    void setInputPorts(InputPort *pLeft, InputPort *pRight);

    float dspLoad() const { return m_dspLoad; }

public slots:

    void start(AudioUnit *pEndOfChain = nullptr);
    void stop();

    void signalUpdateOver();

signals:

    void started();
    void continueGenerateSamples();
    void bufferReady();

    /**
     * Notify DSP load update.
     * @param l
     */
    void dspLoadChanged(float l);

    void signalChanged();

private slots:

    void performChainUpdate();
    float getNextLeftChannelSample();
    float getNextRightChannelSample();

    void generateSamples();

private:

    void setDspLoad(float l);

    void requestSignalUpdate();
    bool isSignalUpdateRequested() const { return m_singnalUpdateRequested; }

    QMutex m_mutex; ///< Protective mutex.

    ISignalChain *m_pSignalChain;

    long m_bufferSize;
    AudioBuffer *m_pLeftBuffer;
    AudioBuffer *m_pRightBuffer;
    float *m_pLeftData;
    float *m_pRightData;

    bool m_started;
    bool m_firstBuffer;

    /// Processing load, [0..1]
    float m_dspLoad;

    /// Signal waveform used for spectrum update
    QVector<float> *m_pSignalBuffer;
    int m_signalIndex;
    bool m_singnalUpdateRequested;

    InputPort *m_pLeftChannelInput;
    InputPort *m_pRightChannelInput;

    QList<AudioUnit*> m_updateChain;
};

#endif // SPEAKERTHREADOBJECT_H

