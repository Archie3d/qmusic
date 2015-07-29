#ifndef MIDIMESSAGE_H
#define MIDIMESSAGE_H

#include <QDebug>
#include <QMetaType>

#include "MidiApi.h"

/**
 * @brief MIDI message representation.
 *
 * This class represents a simple MIDI message (3-bytes).
 * The message raw data is encoded as 4-bytes integer.
 * This class provided method for easy accessing of message content
 * depending on the status.
 */
class QMUSIC_MIDI_API MidiMessage
{
public:

    /**
     * MIDI message status.
     */
    enum Status {
        Status_Invalid, // Non-MIDI, for internal use.

        Status_NoteOff,
        Status_NoteOn,
        Status_PolyphonicAftertouch,
        Status_ControlChange,
        Status_ProgramChange,
        Status_ChannelAftertouch,
        Status_PitchBend,
        Status_System
    };

    /**
     * Controller number (selection).
     */
    enum Controller {
        Controller_BankSelect = 0,
        Controller_Modulation = 1,
        Controller_Breath = 2,
        Controller_Foot = 4,
        Controller_PortamentoTime = 5,
        Controller_DataEntryMSB = 6,
        Controller_MainVolume = 7,
        Controller_Balance = 8,
        Controller_Pan = 10,
        Controller_Effect1 = 12,
        Controller_Effect2 = 13,
        Controller_SustainPedal = 64,
        Controller_Portamento = 65,
        Controller_Sostenuto = 66,
        Controller_SoftPedal = 67,
        Controller_LegatoFootswitch = 68,

        // Channel mode
        Controller_ResetAll = 121,
        Controller_LocalControl = 122,
        Controller_AllNotesOff = 123,
        Controller_OmniOff = 124,
        Controller_OmniOn = 125,
        Controller_MonoOn = 126,
        Controller_PolyOff = Controller_MonoOn,
        Controller_MonoOff = 127,
        Controller_PolyOn = Controller_MonoOn
    };

    // Special system messages
    static const MidiMessage TuneRequest;
    static const MidiMessage EndOfExclusive;
    static const MidiMessage TimingClock;
    static const MidiMessage Start;
    static const MidiMessage Continue;
    static const MidiMessage Stop;
    static const MidiMessage ActiveSensing;
    static const MidiMessage Reset;

    /**
     * Construct an invalid message.
     */
    MidiMessage();
    MidiMessage(unsigned int raw);
    MidiMessage(const MidiMessage &msg);
    MidiMessage& operator =(const MidiMessage &msg);
    bool operator ==(const MidiMessage &mgs) const;
    bool operator !=(const MidiMessage &msg) const;

    unsigned int rawData() const { return m_rawData; }
    unsigned int dataFirstByte() const;
    unsigned int dataSecondByte() const;
    unsigned int dataWord() const;

    bool isValid() const;
    Status status() const;

    /**
     * Returns MIDI channel number [1..16].
     * @return
     */
    unsigned int channel() const;

    /**
     * For note On/Off and polyhponic aftertouch messages returns the note number.
     * @return
     */
    unsigned int noteNumber() const;

    /**
     * For note On/Off and polyphonic aftertouch messages returns the note velocity.
     * @return
     */
    unsigned int velocity() const;

    /**
     * Returns pressure for aftertouch messages.
     * @return
     */
    unsigned int pressure() const;

    unsigned int controllerNumber() const;
    unsigned int controllerValue() const;
    unsigned int programNumber() const;
    unsigned int pitchBend() const;

    // Static methods
    static QString controllerNumberToString(unsigned int ctrl);
    static MidiMessage programChange(unsigned int channel, unsigned int prog);

private:
    unsigned int m_rawData;
};

Q_DECLARE_METATYPE(MidiMessage)

QMUSIC_MIDI_API QDebug operator <<(QDebug dbg, const MidiMessage &msg);

#endif // MIDIMESSAGE_H

