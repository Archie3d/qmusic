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
    m_cutOffFrequency = f;
    update();
}
