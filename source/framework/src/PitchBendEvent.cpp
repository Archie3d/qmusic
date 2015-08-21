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

#include <qmath.h>
#include "PitchBendEvent.h"

PitchBendEvent::PitchBendEvent(int bend)
    : SignalChainEvent(PitchBend),
      m_bend(bend)
{
}

PitchBendEvent::PitchBendEvent(const PitchBendEvent &evt)
    : SignalChainEvent(evt),
      m_bend(evt.m_bend)
{
}

PitchBendEvent& PitchBendEvent::operator =(const PitchBendEvent &evt)
{
    if (this != &evt) {
        SignalChainEvent::operator =(evt);
        m_bend = evt.m_bend;
    }
    return *this;
}

SignalChainEvent* PitchBendEvent::clone() const
{
    return new PitchBendEvent(*this);
}

QString PitchBendEvent::toString() const
{
    return QString("PitchBend (bend=%1)")
            .arg(m_bend);
}

float PitchBendEvent::bendFactor(int semitones)
{
    int bend = m_bend - 8192;
    float dBend = float(bend) / 8192.0f;
    dBend *= float(semitones);
    return pow(2.0, dBend / 12.0);
}
