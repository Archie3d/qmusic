#ifndef AUDIOSINK_H
#define AUDIOSINK_H

#include "AudioUnit.h"
#include "MidiInputDevice.h"

class QtVariantProperty;
class MidiInputDevice;

class MidiIn : public AudioUnit,
               public IMidiInputListener
{
public:

    MidiIn(AudioUnitPlugin *pPlugin);
    ~MidiIn();

protected:

    void processStart();
    void processStop();
    void process();
    void reset();
    void control(const QString &name, const QVariant &value);

    // IMidiInputListener
    void inputMidiMessage(const MidiMessage &msg);

private:

    void createProperties();

    MidiInputDevice *m_pMidiInputDevice;

    bool m_noteOn;
    double m_frequency;
    double m_velocity;

    OutputPort *m_pOutputNoteOn;
    OutputPort *m_pOutputFreq;
    OutputPort *m_pOutputVelocity;

    QtVariantProperty *m_pPropMidiDevice;
    QtVariantProperty *m_pPropChannel;
};

#endif // AUDIOSINK_H

