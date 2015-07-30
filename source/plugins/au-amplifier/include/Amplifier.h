#ifndef AMPLIFIER_H
#define AMPLIFIER_H

#include "AudioUnit.h"

class Amplifier : public AudioUnit
{
public:

    Amplifier(AudioUnitPlugin *pPlugin);
    ~Amplifier();

protected:

    void processStart();
    void processStop();
    void process();
    void reset();
    QGraphicsItem* graphicsItem() override;
    int flags() const override;

private:

    void createProperties();

    InputPort *m_pInput;
    InputPort *m_pGain;
    OutputPort *m_pOutput;
};

#endif // AMPLIFIER_H

