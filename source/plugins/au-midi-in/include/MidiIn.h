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

    // ISerializable interface
    void serialize(QVariantMap &data, SerializationContext *pContext) const;
    void deserialize(const QVariantMap &data, SerializationContext *pContext);

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
    double m_frequencyBend;

    OutputPort *m_pOutputNoteOn;
    OutputPort *m_pOutputFreq;
    OutputPort *m_pOutputVelocity;

    QtVariantProperty *m_pPropPitchBendSemitones;
};

#endif // AU_MIDIIN_H

