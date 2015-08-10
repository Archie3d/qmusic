#ifndef AUDIOBUFFER_H
#define AUDIOBUFFER_H

#include "FrameworkApi.h"

struct AudioBufferPrivate;

/**
 * @brief Rind buffer of float values.
 *
 * The implementation is based on portaudio lock-free ring buffer.
 *
 * Audio buffer holds an array of float values accasible for reading and writing.
 */
class QMUSIC_FRAMEWORK_API AudioBuffer
{
public:

    /**
     * Construct an audio buffer.
     * @param size Buffer size (in samples).
     */
    AudioBuffer(long size);
    ~AudioBuffer();

    /**
     * Returns number of samples that can be written.
     * @return
     */
    long availableToWrite();

    /**
     * Returns number of samples that can be read from the buffer.
     * @return
     */
    long availableToRead();

    /**
     * Read data from the buffer.
     * @param pBuffer Target array where the read data will be placed.
     * @param size Number of samples to be read.
     * @return Number of actually read samples.
     */
    long read(float *pBuffer, long size);

    /**
     * Write data to the buffer.
     * @param pBuffer Source array of samples to be written.
     * @param size Number of samples to write.
     * @return Number of actually written samples.
     */
    long write(const float *pBuffer, long size);

    /**
     * Clear the buffer.
     */
    void clear();

private:

    // Private fields are defined in corresponding .cpp file.
    AudioBufferPrivate *m;
};

#endif // AUDIOBUFFER_H

