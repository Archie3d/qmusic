#ifndef AUDIOSINK_H
#define AUDIOSINK_H

#include "VAOnePoleFilter.h"
#include "AudioUnit.h"

class QtVariantProperty;


class LHPFilter : public AudioUnit
{
public:

    LHPFilter(AudioUnitPlugin *pPlugin);
    ~LHPFilter();

protected:

    void processStart();
    void processStop();
    void process();
    void reset();
    void control(const QString &name, const QVariant &value);

private:

    void createProperties();

    VAOnePoleFilter m_filter;

    InputPort *m_pInput;
    InputPort *m_pInputCutOffFreq;
    OutputPort *m_pOutput;

    QtVariantProperty *m_pFilterType;
};

#endif // AUDIOSINK_H

