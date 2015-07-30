#ifndef AUDIOSINK_H
#define AUDIOSINK_H

#include "AudioUnit.h"

class QtVariantProperty;

class Generator : public AudioUnit
{
public:

    Generator(AudioUnitPlugin *pPlugin);
    ~Generator();

protected:

    void processStart();
    void processStop();
    void process();
    void reset();
    void control(const QString &name, const QVariant &value);

private:

    void createProperties();

    double m_phase;

    InputPort *m_pInputFreq;
    OutputPort *m_pOutput;

    QtVariantProperty *m_pPropWaveform;
    QtVariantProperty *m_pPropFrequency;
};

#endif // AUDIOSINK_H

