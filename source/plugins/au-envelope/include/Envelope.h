#ifndef AMPLIFIER_H
#define AMPLIFIER_H

#include "AudioUnit.h"

class QtVariantProperty;

class Envelope : public AudioUnit
{
public:

    // Envelope state
    enum State {
        State_Off,
        State_Attack,
        State_Decay,
        State_Sustain,
        State_Release
    };


    Envelope(AudioUnitPlugin *pPlugin);
    ~Envelope();

    // ISerializable interface
    void serialize(QVariantMap &data, SerializationContext *pContext) const override;
    void deserialize(const QVariantMap &data, SerializationContext *pContext) override;

protected:

    void processStart();
    void processStop();
    void process();
    void reset();
    void control(const QString &name, const QVariant &value);

private:

    void createProperties();
    void doEnvelope();

    void calculateAttack();
    void calculateDecay();
    void calculateRelease();


    InputPort *m_pNoteOnInput;
    OutputPort *m_pOutput;

    bool m_noteOn;
    State m_state;
    float m_output;

    float m_attackTCO;
    float m_decayTCO;
    float m_releaseTCO;

    float m_attackOffset;
    float m_attackCoeff;
    float m_decayOffset;
    float m_decayCoeff;
    float m_releaseOffset;
    float m_releaseCoeff;

    QtVariantProperty *m_pAttackTimeMs;
    QtVariantProperty *m_pDecayTimeMs;
    QtVariantProperty *m_pSustainLevel;
    QtVariantProperty *m_pReleaseTimeMs;
};

#endif // AMPLIFIER_H

