#ifndef AU_STK_SAXOFONY_H
#define AU_STK_SAXOFONY_H

#include "AudioUnit.h"

class QtVariantProperty;

namespace stk {
    class Saxofony;
}

class StkSaxofony : public AudioUnit
{
public:

    // Control messages
    enum {
        Ctrl_ReedStiffness = 2,
        Ctrl_NoiseGain = 4,
        Ctrl_VibratoFrequency = 29,
        Ctrl_VibratoGain = 1,
        Ctrl_BreathPressure = 128,
        Ctrl_BlowPosition = 11,
        Ctrl_ReedAperture = 26
    };

    StkSaxofony(AudioUnitPlugin *pPlugin);
    ~StkSaxofony();

    QColor color() const override { return QColor(250, 240, 255); }

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

    QtVariantProperty *m_pPropBlowPosition;
    QtVariantProperty *m_pPropReedStiffness;
    QtVariantProperty *m_pPropReedAperture;
    QtVariantProperty *m_pPropNoiseGain;

    stk::Saxofony *m_pSaxofony;
};

#endif // AU_STK_SAXOFONY_H

