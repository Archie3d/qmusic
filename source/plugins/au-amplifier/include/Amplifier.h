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

    InputPortPtr m_inputPtr;
    InputPortPtr m_gainPtr;
    OutputPortPtr m_outputPtr;
};

#endif // AMPLIFIER_H

