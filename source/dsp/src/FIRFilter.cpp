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
#include "FIRFilter.h"

/*
 * This function calculates the zeroth order Bessel function
 */
float ino(float x)
{
    int d = 0;
    float ds = 1.0f;
    float s = 1.0f;
    do {
        d += 2;
        ds *= x*x/(d*d);
        s += ds;
    } while (ds > s*1e-6);
    return s;
}

FIRFilter::FIRFilter()
    : m_coefs(),
      m_pBuffer(nullptr)
{
    m_coefs.append(1.0);
    reset();
}

FIRFilter::FIRFilter(const QVector<float> &coefs)
    : m_coefs(coefs),
      m_pBuffer(nullptr)
{
    Q_ASSERT(coefs.count() > 0);
    reset();
}

FIRFilter::~FIRFilter()
{
    delete[] m_pBuffer;
}

void FIRFilter::reset()
{
    delete[] m_pBuffer;
    m_pBuffer = new float[m_coefs.count()];
    memset(m_pBuffer, 0, m_coefs.count() * sizeof(float));
    m_bufferIndex = 0;
}

float FIRFilter::process(float x)
{
    const float *pCoefs = m_coefs.constData();
    float out = x * pCoefs[0];
    for (int i = m_coefs.count() - 1; i > 0; --i) {
        out += m_pBuffer[i] * pCoefs[i];
        m_pBuffer[i] = m_pBuffer[i - 1];
    }
    m_pBuffer[0] = x;
    return out;
}

FIRFilter* FIRFilter::create(float fa, float fb, float att, float sampleRate, int length)
{
    // From http://www.arc.id.au/FilterDesign.html
    QVector<float> A;
    length = int(length / 2) * 2 + 1;
    int Np = (length - 1) / 2;

    A.append(2.0f * (fb - fa) / sampleRate);
    for (int j = 1; j < Np; j++) {
        A.append((sin(2*j*M_PI*fb/sampleRate) - sin(2*j*M_PI*fa/sampleRate)) / (j*M_PI));
    }

    float alpha = 0.0;
    if (att < 21.0) {
        alpha = 0.0;
    } else if (att > 50.0) {
        alpha = 0.1102 * (att - 8.7);
    } else {
        alpha = 0.5842 * pow((att - 21.0), 0.4) + 0.07886 * (att - 21.0);
    }

    float inoalpha = ino(alpha);

    QVector<float> H;
    for (int i = 0; i < length; i++) {
        H.append(0.0f);
    }

    for (int j = 0; j <= Np; j++) {
        H[Np+j] = A[j]*ino(alpha * sqrt(1-(j*j/(Np*Np)))) / inoalpha;
    }

    for (int j = 0; j < Np; j++) {
        H[j] = H[length - 1 - j];
    }

    return new FIRFilter(H);
}
