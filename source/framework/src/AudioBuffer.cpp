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
#include "portaudio.h"
#include "pa_ringbuffer.h"
#include "pa_util.h"
#include "AudioBuffer.h"

struct AudioBufferPrivate
{
    PaUtilRingBuffer ringBuffer;
    float *pData;
    ring_buffer_size_t size;
};

AudioBuffer::AudioBuffer(long size)
{
    m = new AudioBufferPrivate;
    m->size = size;
    m->pData = new float[size];
    long ret = PaUtil_InitializeRingBuffer(&m->ringBuffer, sizeof(float), size, m->pData);
    if (ret != 0) {
        qCritical() << "Buffer size is not power of two:" << size;
    }

}

AudioBuffer::~AudioBuffer()
{
    delete[] m->pData;
    delete m;
}

long AudioBuffer::availableToWrite()
{
    return PaUtil_GetRingBufferWriteAvailable(&m->ringBuffer);
}

long AudioBuffer::availableToRead()
{
    return PaUtil_GetRingBufferReadAvailable(&m->ringBuffer);
}

long AudioBuffer::read(float *pBuffer, long size)
{
    return PaUtil_ReadRingBuffer(&m->ringBuffer, pBuffer, size);
}

long AudioBuffer::write(const float *pBuffer, long size)
{
    return PaUtil_WriteRingBuffer(&m->ringBuffer, pBuffer, size);
}

void AudioBuffer::clear()
{
    PaUtil_FlushRingBuffer(&m->ringBuffer);
}

