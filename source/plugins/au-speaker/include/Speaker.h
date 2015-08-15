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

#ifndef AU_SPEAKER_H
#define AU_SPEAKER_H

#include <QObject>
#include "AudioBuffer.h"
#include "AudioDevice.h"
#include "AudioUnit.h"

class QTimer;
class QThread;
class SpeakerThreadObject;

class Speaker : public QObject,
                public AudioUnit,
                public IAudioDeviceListener
{
    Q_OBJECT
public:

    enum Channel {
        Channel_Left,
        Channel_Right
    };

    Speaker(AudioUnitPlugin *pPlugin);
    ~Speaker();

    QColor color() const override;

    int flags() const override { return Flag_SingleInstance; }

    void processAudio(const float *pInputBuffer, float *pOutputBuffer, long nSamples) override;

    AudioBuffer* leftBuffer() const;
    AudioBuffer* rightBuffer() const;

protected:

    void processStart();
    void processStop();
    void process();

private:

    void allocateBuffers();
    void releaseBuffers();
    static int bufferSizeFromSettings();

    InputPort *m_pInputLeft;
    InputPort *m_pInputRight;
    QThread *m_pThread;
    SpeakerThreadObject *m_pThreadObject;

    float *m_pLeftBuffer;
    float *m_pRightBuffer;
};

#endif // AU_SPEAKER_H

