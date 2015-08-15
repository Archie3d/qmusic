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

