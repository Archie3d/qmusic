#include <string.h>
#include "../include/DelayLine.h"

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
