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

#include "FilterAbstractImpl.h"

FilterAbstractImpl::FilterAbstractImpl()
    : m_sampleRate(44100.0),
      m_cutOffFrequency(1000.0)
{
}

void FilterAbstractImpl::setSampleRate(double sr)
{
    m_sampleRate = sr;
    update();
}

void FilterAbstractImpl::setCutOffFrequency(double f)
{
    if (m_cutOffFrequency != f) {
        m_cutOffFrequency = f;
        update();
    }
}
