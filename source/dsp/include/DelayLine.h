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
     * @param nSamplesMax Maximum number of samples to be delayed.
     */
    DelayLine(int nSamplesMax = 4096);
    ~DelayLine();

    void allocate(int nSamplesMax);

    /**
     * @brief Set current delay (in samples).
     * This sets delay within 0..nSamplesMax without buffer reallocation.
     * @param nSamples Number of samples to delay
     */
    void setDelay(int nSamples);

    /**
     * Reset the delay line.
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

    /**
     * Returns maximum possible delay (in samples).
     * @return
     */
    int samplesMax() const { return m_nSamplesMax; }

private:

    int m_nSamples;     ///< Number of samples to delay.
    int m_nSamplesMax;  ///< Maximum number of samples.
    double *m_pBuffer;  ///< Samples buffer.
    int m_inIndex;      ///< Index within the buffer for input sample.
    int m_outIndex;     ///< Index within the buffer for output sample.
};

#endif // DELAYLINE_H

