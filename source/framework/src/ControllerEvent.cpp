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

#include "ControllerEvent.h"

const int cMaxControlValue(127);

ControllerEvent::ControllerEvent(int number, int value)
    : SignalChainEvent(Controller),
      m_number(number),
      m_value(value)
{
}

ControllerEvent::ControllerEvent(const ControllerEvent &evt)
    : SignalChainEvent(evt),
      m_number(evt.m_number),
      m_value(evt.m_value)
{
}

ControllerEvent& ControllerEvent::operator =(const ControllerEvent &evt)
{
    if (this != &evt) {
        SignalChainEvent::operator =(evt);
        m_number = evt.m_number;
        m_value = evt.m_value;
    }
    return *this;
}

SignalChainEvent* ControllerEvent::clone() const
{
    return new ControllerEvent(*this);
}

QString ControllerEvent::toString() const
{
    return QString("Controller (number=%1, value=%2)")
            .arg(m_number)
            .arg(m_value);
}

float ControllerEvent::normalizedControlValue() const
{
    return qMin(float(m_number) / float(cMaxControlValue), 1.0f);
}
