#ifndef DELAYLINE_H
#define DELAYLINE_H

#include "DspApi.h"

class QMUSIC_DSP_API DelayLine
{
public:

    DelayLine(int nSamples = 0);
    ~DelayLine();

    void reset();

    double process(double x);

private:

    int m_nSamples;     ///< Number of samples.
    double *m_pBuffer;  ///< Samples buffer.
    int m_index;
};

#endif // DELAYLINE_H

