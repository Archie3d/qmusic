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

#ifndef VAONEPOLEFILTER_H
#define VAONEPOLEFILTER_H

#include "DspApi.h"
#include "FilterAbstractImpl.h"

/**
 * @brief Virtual analogue single pole filter
 */
class QMUSIC_DSP_API VAOnePoleFilter : public FilterAbstractImpl
{
public:

    enum Type {
        Type_LP,    /// Low-pass filter
        Type_HP     /// High-pass filter
    };

    VAOnePoleFilter();

    void reset() override;
    void update() override;
    double doFilter(double x) override;

    void setType(Type t) { m_type = t; }

    void setFeedback(double fb) { m_dFeedback = fb; }
    double getFeedbackOutput() const { return m_dFeedback; }

private:

    void resetFilter();
    void updateFilter();

    Type m_type;

    double m_dAlpha;
    double m_dBeta;
    double m_dZ1;
    double m_dGamma;
    double m_dDelta;
    double m_dEpsilon;
    double m_dA0;
    double m_dFeedback;
};

#endif // VAONEPOLEFILTER_H

