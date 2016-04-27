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

#ifndef AU_INPUT_H
#define AU_INPUT_H

#include "AudioDevice.h"
#include "AudioUnit.h"

class AudioBuffer;
typedef void PaStream;

class Input : public AudioUnit,
              public IAudioDeviceListener
{
public:

    Input(AudioUnitPlugin *pPlugin);
    ~Input();

    QColor color() const override;
    int flags() const override;
    QGraphicsItem* graphicsItem() override;

    void processAudio(const float *pInputBuffer, float *pOutputBuffer, long nSamples) override;

protected:

    void processStart() override;
    void processStop() override;
    void process() override;

private:

    void allocateBuffers();
    void releaseBuffers();
    inline bool isBufferAllocated() const { return m_bufferAllocated; }

    AudioBuffer *m_pLeftBuffer;
    AudioBuffer *m_pRightBuffer;

    OutputPort *m_pOutputLeft;
    OutputPort *m_pOutputRight;

    float *m_pLeft;
    float *m_pRight;

    bool m_bufferAllocated;
};

#endif // AU_INPUT_H

