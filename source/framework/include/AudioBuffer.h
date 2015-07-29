#ifndef AUDIOBUFFER_H
#define AUDIOBUFFER_H

#include "FrameworkApi.h"

struct AudioBufferPrivate;

/**
 * @brief Rind buffer of float values.
 */
class QMUSIC_FRAMEWORK_API AudioBuffer
{
public:
    AudioBuffer(long size);
    ~AudioBuffer();

    long availableToWrite();
    long availableToRead();

    long read(float *pBuffer, long size);
    long write(const float *pBuffer, long size);

    void clear();

private:
    AudioBufferPrivate *m;
};

#endif // AUDIOBUFFER_H

