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

#include <qmath.h>
#include "VAOnePoleFilter.h"

VAOnePoleFilter::VAOnePoleFilter()
{
    m_type = Type_LP;

    m_dAlpha = 1.0;
    m_dBeta = 0.0;
    m_dZ1 = 0.0;
    m_dGamma = 1.0;
    m_dDelta = 0.0;
    m_dEpsilon = 0.0;
    m_dA0 = 1.0;
    m_dFeedback = 0.0;

    resetFilter();
}

void VAOnePoleFilter::reset()
{
    resetFilter();
}

void VAOnePoleFilter::update()
{
    updateFilter();
}

double VAOnePoleFilter::doFilter(double x)
{
    double xn = x * m_dGamma + m_dFeedback + m_dEpsilon * getFeedbackOutput();
    double vn = (m_dA0 * xn - m_dZ1) * m_dAlpha;
    double lpf = vn + m_dZ1;
    m_dZ1 = vn + lpf;
    double hpf = xn - lpf;

    if (m_type == Type_LP) {
        return lpf;
    }

    return hpf;
}

void VAOnePoleFilter::resetFilter()
{
    m_dZ1 = 0.0;
    m_dFeedback = 0.0;
}

void VAOnePoleFilter::updateFilter()
{
    double wd = 2 * M_PI * cutOffFrequency();
    double T = 1.0 / sampleRate();
    double wa = (2.0/T) * tan(wd * T/2);
    double g = wa * T/2;

    m_dAlpha = g / (1.0 + g);
}
