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

private:

    InputPortPtr m_inputAPtr;
    InputPortPtr m_inputBPtr;
    OutputPortPtr m_outputPtr;
};

#endif // AUDIOSINK_H

