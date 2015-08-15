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

    int flags() const override { return Flag_SingleInstance; }

    // ISerializable interface
    void serialize(QVariantMap &data, SerializationContext *pContext) const;
    void deserialize(const QVariantMap &data, SerializationContext *pContext);

protected:

    void processStart();
    void processStop();
    void process();
    void reset();

    // IMidiInputListener
    void inputMidiMessage(const MidiMessage &msg);

private:

    void createProperties();

    bool m_noteOn;
    float m_frequency;
    float m_velocity;
    float m_frequencyBend;
    int m_noteNumber;

    OutputPort *m_pOutputNoteOn;
    OutputPort *m_pOutputFreq;
    OutputPort *m_pOutputVelocity;

    QtVariantProperty *m_pPropPitchBendSemitones;
};

#endif // AU_MIDIIN_H

