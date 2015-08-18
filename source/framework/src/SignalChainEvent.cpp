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

#include <QDebug>
#include "SignalChainEvent.h"

SignalChainEvent::SignalChainEvent(const QString &name, const QVariant &data)
    : m_name(name),
      m_data(data)
{
}

SignalChainEvent::SignalChainEvent(const SignalChainEvent &evt)
    : m_name(evt.m_name),
      m_data(evt.m_data)
{
}

SignalChainEvent& SignalChainEvent::operator =(const SignalChainEvent &evt)
{
    if (this != &evt) {
        m_name = evt.m_name;
        m_data = evt.m_data;
    }
    return *this;
}

QDebug operator <<(QDebug dbg, const SignalChainEvent &evt)
{
    dbg.nospace() << evt.name() << " " << evt.data();
    return dbg.maybeSpace();
}
