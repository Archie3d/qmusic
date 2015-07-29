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

