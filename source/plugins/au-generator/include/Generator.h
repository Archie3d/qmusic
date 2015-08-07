#ifndef AU_GENERATOR_H
#define AU_GENERATOR_H

#include "AudioUnit.h"

class QtVariantProperty;

class Generator : public AudioUnit
{
public:

    Generator(AudioUnitPlugin *pPlugin);
    ~Generator();

    // ISerializable interface
    void serialize(QVariantMap &data, SerializationContext *pContext) const;
    void deserialize(const QVariantMap &data, SerializationContext *pContext);

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

#endif // AU_GENERATOR_H

