#ifndef AUDIOSINK_H
#define AUDIOSINK_H

#include "AudioUnit.h"

class QtVariantProperty;

class Mixer : public AudioUnit
{
public:

    Mixer(AudioUnitPlugin *pPlugin);
    ~Mixer();

protected:

    void processStart();
    void processStop();
    void process();
    QGraphicsItem* graphicsItem() override;
    int flags() const override;

private:

    InputPort *m_pInputA;
    InputPort *m_pInputB;
    OutputPort *m_pOutput;
};

#endif // AUDIOSINK_H

