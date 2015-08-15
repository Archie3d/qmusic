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

#ifndef IFILTER_H
#define IFILTER_H

#include "DspApi.h"

/**
 * @brief Interface to a filter.
 */
class QMUSIC_DSP_API IFilter
{
public:

    /**
     * Reset filter internal states and memory.
     */
    virtual void reset() = 0;

    /**
     * Update filter coefficients.
     */
    virtual void update() = 0;

    /**
     * Set filter sample rate.
     * @param sr
     */
    virtual void setSampleRate(double sr) = 0;

    /**
     * Set filter cut-off frequency.
     * @param f
     */
    virtual void setCutOffFrequency(double f) = 0;

    /**
     * Process a single sample.
     * @param x
     * @return
     */
    virtual double doFilter(double x) = 0;

    /// Destructor.
    virtual ~IFilter() {}
};

#endif // IFILTER_H

