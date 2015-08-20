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

#include "OutputPortProxy.h"

OutputPortProxy::OutputPortProxy()
    : OutputPort(),
      m_pOutputPort(nullptr)
{
}

OutputPortProxy::OutputPortProxy(OutputPort *pOutput)
    : OutputPort(pOutput->name()),
      m_pOutputPort(pOutput)
{
}

OutputPortProxy::OutputPortProxy(const QString &name, OutputPort *pOutput)
    : OutputPort(name),
      m_pOutputPort(pOutput)
{
}

float OutputPortProxy::value() const
{
    return m_pOutputPort == nullptr ? 0.0f : m_pOutputPort->value();
}

void OutputPortProxy::update()
{
    if (m_pOutputPort != nullptr) {
        update();
    }
}
