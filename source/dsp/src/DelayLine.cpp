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

#include <string.h>
#include "DelayLine.h"

DelayLine::DelayLine(int nSamples)
    : m_nSamples(nSamples),
      m_pBuffer(nullptr)
{
    if (nSamples > 0) {
        m_pBuffer = new double[m_nSamples];
    }
    reset();
}

DelayLine::~DelayLine()
{
    delete[] m_pBuffer;
}

void DelayLine::reset()
{
    m_index = 0;
    memset(m_pBuffer, 0, sizeof(double) * m_nSamples);
}

double DelayLine::process(double x)
{
    if (m_pBuffer == nullptr || m_nSamples == 0) {
        return x;
    }
    m_pBuffer[m_index] = x;    
    m_index = (m_index + 1) % m_nSamples;
    return m_pBuffer[m_index];
}
