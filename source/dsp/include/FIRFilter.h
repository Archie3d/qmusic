#ifndef FIRFILTER_H
#define FIRFILTER_H

#include <QVector>
#include "DspApi.h"

/**
 * A basic implementation fo a FIR filter.
 */
class QMUSIC_DSP_API FIRFilter
{
public:

    FIRFilter();
    FIRFilter(const QVector<float> &coefs);
    ~FIRFilter();

    /**
     * Reset the filter by setting the internal buffer to zero.
     */
    void reset();

    /**
     * Process the next sample
     * @param x Input sample.
     * @return Output (filtered) value.
     */
    float process(float x);

    FIRFilter* create(float fa, float fb, float att, float sampleRate, int length = 127);

private:

    Q_DISABLE_COPY(FIRFilter)

    QVector<float> m_coefs;    ///< Filter coefficients
    float *m_pBuffer;          ///< Samples buffer;
    int m_bufferIndex;          ///< Position in samples buffer.
};

#endif // FIRFILTER_H

