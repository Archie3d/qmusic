#ifndef DELAYLINE_H
#define DELAYLINE_H

#include "DspApi.h"

/**
 * Signal delay line.
 *
 * Delay line keeps an internal samples buffer used to
 * delay the signal propagation.
 */
class QMUSIC_DSP_API DelayLine
{
public:

    /**
     * Construct delay line.
     * @param nSamples Number of samples to be delayed.
     */
    DelayLine(int nSamples = 0);
    ~DelayLine();

    /**
     * Reset the delat line.
     * This will clear the internal buffer by setting all the
     * samples to zero.
     */
    void reset();

    /**
     * Process the delay line.
     * @param x Incoming sample.
     * @return Outgoing sample (delayed).
     */
    double process(double x);

private:

    int m_nSamples;     ///< Number of samples.
    double *m_pBuffer;  ///< Samples buffer.
    int m_index;        ///< Index within the buffer
};

#endif // DELAYLINE_H

