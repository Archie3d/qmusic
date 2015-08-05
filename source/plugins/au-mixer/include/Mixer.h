#ifndef AU_MIXER_H
#define AU_MIXER_H

#include <QList>
#include "AudioUnit.h"

class QtVariantProperty;

class Mixer : public AudioUnit
{
public:

    Mixer(AudioUnitPlugin *pPlugin);
    ~Mixer();

protected:

    void processStart() override;
    void processStop() override;
    void process() override;

private:

    QList<InputPort*> m_inputs;
    OutputPort *m_pOutput;
};

#endif // AU_ADDER_H

