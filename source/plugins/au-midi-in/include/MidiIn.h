#ifndef AU_MIDIIN_H
#define AU_MIDIIN_H

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

    QColor color() const override;

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

    bool m_noteOn;
    double m_frequency;
    double m_velocity;

    OutputPort *m_pOutputNoteOn;
    OutputPort *m_pOutputFreq;
    OutputPort *m_pOutputVelocity;
};

#endif // AU_MIDIIN_H

