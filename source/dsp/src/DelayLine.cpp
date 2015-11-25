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

#include <qglobal.h>
#include "DelayLine.h"

DelayLine::DelayLine(int nSamplesMax)
    : m_nSamples(0),
      m_nSamplesMax(nSamplesMax),
      m_pBuffer(nullptr)
{
    Q_ASSERT(nSamplesMax > 0);

    m_nSamples = 0;
    m_inIndex = 0;
    m_outIndex = 0;

    allocate(nSamplesMax);
}

DelayLine::~DelayLine()
{
    delete[] m_pBuffer;
}

void DelayLine::allocate(int nSamplesMax)
{
    Q_ASSERT(nSamplesMax > 0);
    if (m_pBuffer == nullptr) {
        // First time allocation
        m_nSamplesMax = nSamplesMax;
        m_pBuffer = new double[m_nSamplesMax];
    } else {
        // Buffer is already allocated
        if (m_nSamplesMax != nSamplesMax) {
            // But of a different size, reallocate the buffer
            delete[] m_pBuffer;
            m_nSamplesMax = nSamplesMax;
            m_pBuffer = new double[m_nSamplesMax];
        }
    }
    reset();
}

void DelayLine::setDelay(int nSamples)
{
    int delay = qMin(nSamples, m_nSamplesMax - 1);
    if (delay == m_nSamples) {
        // No changes
        return;
    }

    m_nSamples = delay;

    if (m_inIndex >= m_nSamples) {
        m_outIndex = m_inIndex - m_nSamples;
    } else {
        m_outIndex = m_nSamplesMax - m_nSamples + m_inIndex;
    }
}

void DelayLine::reset()
{
    Q_ASSERT(m_pBuffer != nullptr);
    memset(m_pBuffer, 0, sizeof(double) * m_nSamplesMax);
    m_inIndex = 0;
    m_outIndex = 0;
    setDelay(m_nSamples);
}

double DelayLine::process(double x)
{
    if (m_pBuffer == nullptr || m_nSamples == 0) {
        return x;
    }

    m_pBuffer[m_inIndex] = x;
    float out = m_pBuffer[m_outIndex];

    m_inIndex = (m_inIndex + 1) % m_nSamplesMax;
    m_outIndex = (m_outIndex + 1) % m_nSamplesMax;

    return out;
}
