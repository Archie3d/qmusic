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

#include "SerializationContext.h"
#include "AudioUnit.h"
#include "OutputPort.h"
#include "InputPort.h"

InputPort::InputPort()
    : Port(Direction_Input),
      m_pConnectedOutputPort(nullptr),
      m_defaultValue(0.0f)
{
}

InputPort::InputPort(const QString &name, float defaultValue)
    : Port(Direction_Input, name),
      m_pConnectedOutputPort(nullptr),
      m_defaultValue(defaultValue)
{
}

float InputPort::value() const
{
    return m_pConnectedOutputPort == nullptr ? m_defaultValue : m_pConnectedOutputPort->value();
}

void InputPort::update()
{
    if (m_pConnectedOutputPort != nullptr) {
        m_pConnectedOutputPort->update();
    }
}

int InputPort::index() const
{
    AudioUnit *pAu = dynamic_cast<AudioUnit*>(audioUnit());
    Q_ASSERT(pAu != nullptr);

    const QList<InputPort*> &inputs = pAu->inputs();
    for (int i = 0; i < inputs.count(); ++i) {
        if (inputs[i] == this) {
            return i;
        }
    }
    return -1;
}

void InputPort::connect(OutputPort *pOutput)
{
    m_pConnectedOutputPort = pOutput;
}

void InputPort::disconnect()
{
    m_pConnectedOutputPort = nullptr;
}
