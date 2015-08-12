#ifndef AU_DELAY_H
#define AU_DELAY_H

#include "AudioUnit.h"

class DelayLine;
class QtVariantProperty;

class Delay : public AudioUnit
{
public:

    Delay(AudioUnitPlugin *pPlugin);
    ~Delay();

    // ISerializable interface
    void serialize(QVariantMap &data, SerializationContext *pContext) const;
    void deserialize(const QVariantMap &data, SerializationContext *pContext);

protected:

    void processStart();
    void processStop();
    void process();
    void reset();

private:

    void createProperties();

    InputPort *m_pInput;
    OutputPort *m_pOutput;

    DelayLine *m_pDelayLine;

    QtVariantProperty *m_pPropDelay;
};

#endif // AU_DELAY_H

