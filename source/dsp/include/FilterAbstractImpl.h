#ifndef FILTERABSTRACTIMPL_H
#define FILTERABSTRACTIMPL_H

#include "DspApi.h"
#include "IFilter.h"

class QMUSIC_DSP_API FilterAbstractImpl : public IFilter
{
public:

    FilterAbstractImpl();

    void setSampleRate(double sr) override;
    void setCutOffFrequency(double f) override;

protected:

    double sampleRate() const { return m_sampleRate; }
    double cutOffFrequency() const { return m_cutOffFrequency; }

private:

    double m_sampleRate;
    double m_cutOffFrequency;
};

#endif // FILTERABSTRACTIMPL_H

