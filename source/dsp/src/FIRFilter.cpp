#include "FIRFilter.h"

FIRFilter::FIRFilter()
    : m_coefs(),
      m_pBuffer(nullptr)
{
    m_coefs.append(1.0);
    reset();
}
k
FIRFilter::FIRFilter(const QVector<double> &coefs)
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
    m_pBuffer = new double[m_coefs.count()];
    memset(m_pBuffer, 0, m_coefs.count() * sizeof(double));
    m_bufferIndex = 0;
}

double FIRFilter::process(double x)
{
    // enqueue data into the buffer
    m_pBuffer[m_bufferIndex] = x;

    // Convolute filter coefficients and buffered samples.
    double out = 0.0;
    int j = m_bufferIndex;
    for (int i = 0; i < m_coefs.count(); ++i) {
        out += m_pBuffer[j++] * m_coefs[i];
        if (j >= m_coefs.count()) {
            j = 0;
        }
    }

    if (--m_bufferIndex < 0) {
        m_bufferIndex = m_coefs.count() - 1;
    }

    return out;
}
