#ifndef AU_STK_BRASS_H
#define AU_STK_BRASS_H

#include "AudioUnit.h"

class QtVariantProperty;

namespace stk {
    class Brass;
}

class StkBrass : public AudioUnit
{
public:

    // Control messages
    enum {
        Ctrl_LipTension = 2,
        Ctrl_SlideLength = 4,
        Ctrl_BreathPressure = 128
    };


    StkBrass(AudioUnitPlugin *pPlugin);
    ~StkBrass();

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

    QtVariantProperty *m_pPropLipTension;
    QtVariantProperty *m_pPropSlideLength;

    stk::Brass *m_pBrass;
};

#endif // AU_STK_BRASS_H

