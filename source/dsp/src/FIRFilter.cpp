#include "FIRFilter.h"

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
    for (int i = m_coefs.count() - 1; i > 0; i--) {
        out += m_pBuffer[i] * pCoefs[i];
        m_pBuffer[i] = m_pBuffer[i - 1];
    }
    m_pBuffer[0] = x;
    return out;
}
