#ifndef MULTIPLIER_H
#define MULTIPLIER_H

#include "AudioUnit.h"

class Multiplier : public AudioUnit
{
public:

    Multiplier(AudioUnitPlugin *pPlugin);
    ~Multiplier();

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

#endif // MULTIPLIER_H

