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

#ifndef FILTERABSTRACTIMPL_H
#define FILTERABSTRACTIMPL_H

#include "DspApi.h"
#include "IFilter.h"

/**
 * Absttract implementation of IFilter interface.
 */
class QMUSIC_DSP_API FilterAbstractImpl : public IFilter
{
public:

    FilterAbstractImpl();

    void setSampleRate(double sr) override;
    void setCutOffFrequency(double f) override;

protected:

    double sampleRate() const { return m_sampleRate; }
    double cutOffFrequency() const { return m_cutOffFrequency; }

private:

    double m_sampleRate;
    double m_cutOffFrequency;
};

#endif // FILTERABSTRACTIMPL_H

