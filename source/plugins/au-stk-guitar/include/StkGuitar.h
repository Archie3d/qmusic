#ifndef AU_STK_GUITAR_H
#define AU_STK_GUITAR_H

#include "AudioUnit.h"

class QtVariantProperty;

namespace stk {
    class Guitar;
}

class StkGuitar : public AudioUnit
{
public:

    enum {
        Ctrl_CouplingGain = 2,
        Ctrl_PickPosition = 4,
        Ctrl_StringDumping = 11,
        Ctrl_Modulation = 1,
        Ctrl_Aftertouch = 128
    };

    StkGuitar(AudioUnitPlugin *pPlugin);
    ~StkGuitar();

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

    QtVariantProperty *m_pPropPickPosition;
    QtVariantProperty *m_pPropStringDumping;

    stk::Guitar *m_pGuitar;
};

#endif // AU_STK_GUITAR_H

