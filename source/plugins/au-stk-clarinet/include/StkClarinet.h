#ifndef AU_STK_CLARINET_H
#define AU_STK_CLARINET_H

#include "AudioUnit.h"

class QtVariantProperty;

namespace stk {
    class Clarinet;
}

class StkClarinet : public AudioUnit
{
public:

    // Control messages
    enum {
        Ctrl_ReedStiffness = 2,
        Ctrl_NoiseGain = 4,
        Ctrl_VibratoFrequency = 11,
        Ctrl_VibratoGain = 1,
        Ctrl_BreathPressure = 128
    };

    StkClarinet(AudioUnitPlugin *pPlugin);
    ~StkClarinet();

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

    InputPort *m_pInputFreq;
    InputPort *m_pInputVelocity;
    InputPort *m_pInputBreath;
    InputPort *m_pInputNoteOn;

    OutputPort *m_pOutput;

    bool m_noteOn;

    QtVariantProperty *m_pPropReedStiffness;
    QtVariantProperty *m_pPropNoiseGain;

    stk::Clarinet *m_pClarinet;
};

#endif // AU_STK_CLARINET_H

