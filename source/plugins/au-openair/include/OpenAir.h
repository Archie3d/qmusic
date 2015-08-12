#ifndef AU_OPENAIR_H
#define AU_OPENAIR_H

#include "AudioUnit.h"

class QtVariantProperty;
class FIRFilter;

class OpenAir : public AudioUnit
{
public:

    OpenAir(AudioUnitPlugin *pPlugin);
    ~OpenAir();

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
    void createFIR();

    InputPort *m_pInput;
    OutputPort *m_pOutput;

    QtVariantProperty *m_pPropEnvironment;

    FIRFilter *m_pFIRFilter;
};

#endif // AU_OPENAIR_H

