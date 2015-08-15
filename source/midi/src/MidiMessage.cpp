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

#include <QHash>
#include "midimessage.h"

const MidiMessage MidiMessage::TuneRequest(0x00F60000);
const MidiMessage MidiMessage::EndOfExclusive(0x00F70000);
const MidiMessage MidiMessage::TimingClock(0x00F80000);
const MidiMessage MidiMessage::Start(0x00FA0000);
const MidiMessage MidiMessage::Continue(0x00FB0000);
const MidiMessage MidiMessage::Stop(0x00FC0000);
const MidiMessage MidiMessage::ActiveSensing(0x00FE0000);
const MidiMessage MidiMessage::Reset(0x00FF0000);

const QHash<MidiMessage::Status, QString> midiStatusToString = []() {
    QHash<MidiMessage::Status, QString> map;
    map[MidiMessage::Status_Invalid] = "Invalid";
    map[MidiMessage::Status_NoteOff] = "Note Off";
    map[MidiMessage::Status_NoteOn] = "Note On";
    map[MidiMessage::Status_PolyphonicAftertouch] = "Polyphonic Aftertouch";
    map[MidiMessage::Status_ControlChange] = "Control Change";
    map[MidiMessage::Status_ProgramChange] = "Program Change";
    map[MidiMessage::Status_ChannelAftertouch] = "Channel Aftertouch";
    map[MidiMessage::Status_PitchBend] = "Pitch Bend";
    map[MidiMessage::Status_System] = "System";
    return map;
}();

MidiMessage::MidiMessage()
    : m_rawData(0)
{
}

MidiMessage::MidiMessage(unsigned int raw)
    : m_rawData(raw)
{
}

MidiMessage::MidiMessage(const MidiMessage &msg)
    : m_rawData(msg.m_rawData)
{
}

MidiMessage& MidiMessage::operator =(const MidiMessage &msg)
{
    if (this != &msg) {
        m_rawData = msg.m_rawData;
    }
    return *this;
}

bool MidiMessage::operator ==(const MidiMessage &msg) const
{
    return m_rawData == msg.m_rawData;
}

bool MidiMessage::operator !=(const MidiMessage &msg) const
{
    return m_rawData != msg.m_rawData;
}

unsigned int MidiMessage::dataFirstByte() const
{
    return (m_rawData & 0x00007F00) >> 8;
}

unsigned int MidiMessage::dataSecondByte() const
{
    return m_rawData & 0x0000007F;
}

unsigned int MidiMessage::dataWord() const
{
    unsigned int lsb = (m_rawData & 0x00007F00) >> 8;
    unsigned int msb = m_rawData & 0x0000007F;

    return (msb << 7) | lsb;
}

bool MidiMessage::isValid() const
{
    return status() != Status_Invalid;
}

MidiMessage::Status MidiMessage::status() const
{
    Status s = Status_Invalid;
    switch ((m_rawData & 0x00F00000) >> 16) {
    case 0x80:
        s = Status_NoteOff;
        break;
    case 0x90:
        s = Status_NoteOn;
        break;
    case 0xA0:
        s = Status_PolyphonicAftertouch;
        break;
    case 0xB0:
        s = Status_ControlChange;
        break;
    case 0xC0:
        s = Status_ProgramChange;
        break;
    case 0xD0:
        s = Status_ChannelAftertouch;
        break;
    case 0xE0:
        s = Status_PitchBend;
        break;
    case 0xF0:
        s = Status_System;
        break;
    default:
        break;
    }

    return s;
}

unsigned int MidiMessage::channel() const
{
    return ((m_rawData & 0x000F0000) >> 16) + 1;
}

unsigned int MidiMessage::noteNumber() const
{
    return (m_rawData & 0x00007F00) >> 8;
}

unsigned int MidiMessage::velocity() const
{
    return (m_rawData & 0x0000007F);
}

unsigned int MidiMessage::pressure() const
{
    unsigned int p = 0;
    Status s = status();
    switch (s) {
    case Status_PolyphonicAftertouch:
        p = m_rawData & 0x0000007F;
        break;
    case Status_ChannelAftertouch:
        p = (m_rawData & 0x00007F00) >> 8;
        break;
    }
    return p;
}

unsigned int MidiMessage::controllerNumber() const
{
    return (m_rawData & 0x00007F00) >> 8;
}

unsigned int MidiMessage::controllerValue() const
{
    return m_rawData & 0x0000007F;
}

unsigned int MidiMessage::programNumber() const
{
    return (m_rawData & 0x00007F00) >> 8;
}

unsigned int MidiMessage::pitchBend() const
{
    return dataWord();
}

QString MidiMessage::controllerNumberToString(unsigned int ctrl)
{
    QString str;
    switch (ctrl) {
    case Controller_BankSelect: str = "Bank select"; break;
    case Controller_Modulation: str = "Modulation"; break;
    case Controller_Breath: str = "Breath"; break;
    case Controller_Foot: str = "Foot"; break;
    case Controller_PortamentoTime: str = "Portamento time"; break;
    case Controller_MainVolume: str = "Main volume"; break;
    case Controller_Balance: str = "Balance"; break;
    case Controller_Pan: str = "Pan"; break;
    case Controller_SustainPedal: str = "Sustain pedal"; break;
    case Controller_Portamento: str = "Portamento"; break;
    case Controller_Sostenuto: str = "Sostenuto"; break;
    case Controller_SoftPedal: str = "Soft pedal"; break;
    case Controller_LegatoFootswitch: str = "Legato footswitch"; break;
    case Controller_ResetAll: str = "Reset all"; break;
    case Controller_LocalControl: str = "Local control"; break;
    case Controller_AllNotesOff: str = "All notes off"; break;
    case Controller_OmniOff:  str = "Omni off"; break;
    case Controller_OmniOn: str = "Omni on"; break;
    case Controller_MonoOn: str = "Mono on"; break;
    case Controller_MonoOff: str = "Mono off"; break;
    default:
        str =  QString::number(ctrl);
    }
    return str;
}

MidiMessage MidiMessage::programChange(unsigned int channel, unsigned int prog)
{
    unsigned int ch = (channel & 0x07) << 16;
    unsigned int prg = (prog & 0x07) << 8;
    unsigned int raw = 0x00C00000 | ch | prg;
    return MidiMessage(raw);
}

QDebug operator <<(QDebug dbg, const MidiMessage &msg)
{
    dbg.nospace() << msg.channel() << " ";
    switch (msg.status()) {
    case MidiMessage::Status_Invalid:
        dbg.nospace() << "Invalid";
        break;
    case MidiMessage::Status_NoteOff:
        dbg.nospace() << "Note Off (note:" << msg.noteNumber() << ", velocity:" << msg.velocity() << ")";
        break;
    case MidiMessage::Status_NoteOn:
        dbg.nospace() << "Note On (note:" << msg.noteNumber() << ", velocity:" << msg.velocity() << ")";
        break;
    case MidiMessage::Status_PolyphonicAftertouch:
        dbg.nospace() << "Polyphonic Aftertouch (note:" << msg.noteNumber() << ", pressure:" << msg.pressure() << ")";
        break;
    case MidiMessage::Status_ControlChange:
        dbg.nospace() << "Control Change (controller:" << MidiMessage::controllerNumberToString(msg.controllerNumber())
                      << ", value:" << msg.controllerValue() << ")";
        break;
    case MidiMessage::Status_ProgramChange:
        dbg.nospace() << "Program Change (program:" << msg.programNumber() << ")";
        break;
    case MidiMessage::Status_ChannelAftertouch:
        dbg.nospace() << "Channel Aftertouch (pressure:" << msg.pressure() << ")";
        break;
    case MidiMessage::Status_PitchBend:
        dbg.nospace() << "Pitch Bend (bend:" << msg.pitchBend() << ")";
        break;
    case MidiMessage::Status_System:
        dbg.nospace() << "System";
        break;
    default:
        break;
    }

    return dbg.maybeSpace();
}
