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

#include "MidiNote.h"
#include "NoteOnEvent.h"

const int cMaxVelocity(127);

NoteOnEvent::NoteOnEvent(int number, int velocity)
    : SignalChainEvent(NoteOn),
      m_number(number),
      m_velocity(velocity)
{
}

NoteOnEvent::NoteOnEvent(const NoteOnEvent &evt)
    : SignalChainEvent(evt),
      m_number(evt.m_number),
      m_velocity(evt.m_velocity)
{
}

NoteOnEvent& NoteOnEvent::operator =(const NoteOnEvent &evt)
{
    if (this != &evt) {
        SignalChainEvent::operator =(evt);
        m_number = evt.m_number;
        m_velocity = evt.m_velocity;
    }
    return *this;
}

SignalChainEvent* NoteOnEvent::clone() const
{
    return new NoteOnEvent(*this);
}

QString NoteOnEvent::toString() const
{
    return QString("NoteOn (note=%1, velocity=%2)")
            .arg(m_number)
            .arg(m_velocity);
}

float NoteOnEvent::frequency() const
{
    return MidiNote(m_number).frequency();
}

float NoteOnEvent::normalizedVelocity() const
{
    return qMin(float(m_velocity) / float(cMaxVelocity), 1.0f);
}
