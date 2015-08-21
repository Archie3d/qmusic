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

#include <QMap>
#include "SignalChainEvent.h"

QMap<SignalChainEvent::Type, QString> cEventTypeToStringMap = []() {
    QMap<SignalChainEvent::Type, QString> map;
    map[SignalChainEvent::Invalid] = "Invalid";
    map[SignalChainEvent::NoteOn] = "NoteOn";
    map[SignalChainEvent::NoteOff] = "NoteOff";
    map[SignalChainEvent::PitchBend] = "PitchBend";
    map[SignalChainEvent::Controller] = "Controller";
    return map;
}();

SignalChainEvent::SignalChainEvent(SignalChainEvent::Type type)
    : m_type(type)
{
}

SignalChainEvent::SignalChainEvent(const SignalChainEvent &evt)
    : m_type(evt.m_type)
{
}

SignalChainEvent& SignalChainEvent::operator =(const SignalChainEvent &evt)
{
    if (this != &evt) {
        m_type = evt.m_type;
    }
    return *this;
}

QString SignalChainEvent::toString() const
{
    return SignalChainEvent::eventTypeToString(m_type);
}

QString SignalChainEvent::eventTypeToString(Type type)
{
    return cEventTypeToStringMap.value(type, QString::number(int(type)));
}
