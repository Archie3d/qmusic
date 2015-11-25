/*
                          qmusic

    Copyright (C) 2015 Arthur Benilov,
    arthur.benilov@gmail.com

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.
*/

#ifndef MIDINOTE_H
#define MIDINOTE_H

#include <QDebug>
#include <QMetaType>

#include "FrameworkApi.h"

/**
 * @brief A note according to the MIDI standard.
 *
 * A note is defined by its number (in MIDI). This class
 * provides some helper methods to map notes to their frequencies.
 * Equally tempered tuning is used for frequency mapping.
 */
class QMUSIC_FRAMEWORK_API MidiNote
{
public:

    /// Musical note.
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

    /**
     * Returns note number.
     * @return
     */
    int number() const { return m_number; }

    /**
     * Returns note frequence according to the equally
     * tempered tuning.
     * @return
     */
    double frequency() const;

    /**
     * Tells the note, discarding its octave.
     * @return
     */
    Note note() const;

    /**
     * Tells whether the note is natural (neither flat nor sharp).
     * @return true if note is natiral;
     */
    bool isNatural() const;

    /**
     * Tells whether the note has a corresponding flat note.
     * @return
     */
    bool hasFlat() const;

    /**
     * Tells whether the note has a corresponding sharp note.
     * @return
     */
    bool hasSharp() const;

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

    /**
     * Returns textual representation of this note, e.g. "C4".
     * @return
     */
    QString toString() const;

private:

    /// Clamp note number to MIDI [0..127] range.
    void clamp();

    /// Note number.
    int m_number;
};

Q_DECLARE_METATYPE(MidiNote)

QDebug operator <<(QDebug dbg, const MidiNote &n);

#endif // MIDINOTE_H

