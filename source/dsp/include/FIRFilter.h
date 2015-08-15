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

#ifndef FIRFILTER_H
#define FIRFILTER_H

#include <QVector>
#include "DspApi.h"

/**
 * A basic implementation fo a FIR filter.
 */
class QMUSIC_DSP_API FIRFilter
{
public:

    FIRFilter();
    FIRFilter(const QVector<float> &coefs);
    ~FIRFilter();

    /**
     * Reset the filter by setting the internal buffer to zero.
     */
    void reset();

    /**
     * Process the next sample
     * @param x Input sample.
     * @return Output (filtered) value.
     */
    float process(float x);

    /**
     * Create a LP/HP/BP filter
     * @param fa Lower frequency
     * @param fb  Upper frequency
     * @param att Attenuation (dB)
     * @param sampleRate Sample rate, e.g. 44100.0
     * @param length Number (odd) of filter coefficients to be produced
     * @return Pointer to create filter.
     */
    FIRFilter* create(float fa, float fb, float att, float sampleRate, int length = 127);

private:

    Q_DISABLE_COPY(FIRFilter)

    QVector<float> m_coefs;    ///< Filter coefficients
    float *m_pBuffer;          ///< Samples buffer;
    int m_bufferIndex;          ///< Position in samples buffer.
};

#endif // FIRFILTER_H

