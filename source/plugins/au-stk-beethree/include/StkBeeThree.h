#ifndef AU_STK_BEETHREE_H
#define AU_STK_BEETHREE_H

#include "AudioUnit.h"

class QtVariantProperty;

namespace stk {
    class BeeThree;
}

class StkBeeThree : public AudioUnit
{
public:

    enum {
        Ctrl_Operator4 = 2,
        Ctrl_Operator3 = 4,
        Ctrl_LFOSpeed = 11,
        Ctrl_LFODepth = 1
    };


    StkBeeThree(AudioUnitPlugin *pPlugin);
    ~StkBeeThree();

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
    InputPort *m_pInputNoteOn;

    OutputPort *m_pOutput;

    bool m_noteOn;

    QtVariantProperty *m_pPropOperator4;
    QtVariantProperty *m_pPropOperator3;
    QtVariantProperty *m_pPropLFOSpeed;
    QtVariantProperty *m_pPropLFODepth;

    stk::BeeThree *m_pBeeThree;
};

#endif // AU_STK_BEETHREE_H

