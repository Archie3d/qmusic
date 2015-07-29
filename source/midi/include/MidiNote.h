#ifndef MIDINOTE_H
#define MIDINOTE_H

#include <QDebug>
#include <QMetaType>

#include "MidiApi.h"

class QMUSIC_MIDI_API MidiNote
{
public:

    enum Note {
        Note_C  = 0,
        Note_Db = 1,
        Note_D  = 2,
        Note_Eb = 3,
        Note_E  = 4,
        Note_F  = 5,
        Note_Gb = 6,
        Note_G  = 7,
        Note_Ab = 8,
        Note_A  = 9,
        Note_Bb = 10,
        Note_B  = 11
    };

    /**
     * Create C4 (middle-octave).
     */
    MidiNote();

    /**
     * Construct a note.
     * Note number will be limited to [0..127] range.
     * @param number MIDI note number.
     */
    MidiNote(int number);
    MidiNote(Note note, int octave);
    MidiNote(const MidiNote &note);
    MidiNote& operator =(const MidiNote &note);
    MidiNote& operator =(int n);
    bool operator ==(const MidiNote &note) const;
    bool operator !=(const MidiNote &note) const;
    bool operator <(const MidiNote &note) const;
    bool operator <=(const MidiNote &note) const;
    bool operator >(const MidiNote &note) const;
    bool operator >=(const MidiNote &note) const;
    MidiNote operator +(int n) const;
    MidiNote operator -(int n) const;
    MidiNote& operator +=(int n);
    MidiNote& operator -=(int n);

    int number() const { return m_number; }
    double frequency() const;
    Note note() const;

    /**
     * Returns note's octave number [-1..9].
     * @return
     */
    int octave() const;

    /**
     * Create note, which is one semi-tone higher than this one.
     * @return
     */
    MidiNote sharp() const;

    /**
     * Create note, which is one semi-tone lower than this one.
     * @return
     */
    MidiNote flat() const;

    QString toString() const;

private:

    void clamp();

    int m_number;
};

Q_DECLARE_METATYPE(MidiNote)

QDebug operator <<(QDebug dbg, const MidiNote &n);

#endif // MIDINOTE_H

