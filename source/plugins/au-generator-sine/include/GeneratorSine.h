#ifndef AU_GENERATOR_SINE_H
#define AU_GENERATOR_SINE_H

#include "AudioUnit.h"

class QtVariantProperty;

class GeneratorSine : public AudioUnit
{
public:

    GeneratorSine(AudioUnitPlugin *pPlugin);

    QColor color() const;

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

    QtVariantProperty *m_pPropFreqScale;
    QtVariantProperty *m_pPropAmplitude;
    QtVariantProperty *m_pPropPhase;
};

#endif // AU_GENERATOR_SINE_H

