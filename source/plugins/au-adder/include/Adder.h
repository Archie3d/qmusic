#ifndef AU_ADDER_H
#define AU_ADDER_H

#include "AudioUnit.h"

class Adder : public AudioUnit
{
public:

    Adder(AudioUnitPlugin *pPlugin);
    ~Adder();

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

#endif // AU_ADDER_H

