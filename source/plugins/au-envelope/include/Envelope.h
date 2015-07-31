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

    void deserialize(const QVariantMap &data, SerializationContext *pContext);

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
    double m_output;

    double m_attackTCO;
    double m_decayTCO;
    double m_releaseTCO;

    double m_attackOffset;
    double m_attackCoeff;
    double m_decayOffset;
    double m_decayCoeff;
    double m_releaseOffset;
    double m_releaseCoeff;

    QtVariantProperty *m_pAttackTimeMs;
    QtVariantProperty *m_pDecayTimeMs;
    QtVariantProperty *m_pSustainLevel;
    QtVariantProperty *m_pReleaseTimeMs;
};

#endif // AMPLIFIER_H

