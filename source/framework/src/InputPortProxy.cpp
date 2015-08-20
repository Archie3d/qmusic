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

#include "InputPortProxy.h"

InputPortProxy::InputPortProxy()
    : InputPort(),
      m_pInputPort(nullptr)
{
}

InputPortProxy::InputPortProxy(InputPort *pInput)
    : InputPort(pInput->name()),
      m_pInputPort(pInput)
{
}

InputPortProxy::InputPortProxy(const QString &name, InputPort *pInput)
    : InputPort(name),
      m_pInputPort(pInput)
{
}

float InputPortProxy::value() const
{
    return m_pInputPort == nullptr ? 0.0f : m_pInputPort->value();
}

void InputPortProxy::update()
{
    if (m_pInputPort != nullptr) {
        m_pInputPort->update();
    }
}
