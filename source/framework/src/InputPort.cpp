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
      m_defaultValue(0.0f),
      m_pValue(&m_defaultValue)
{
}

InputPort::InputPort(const QString &name, float defaultValue)
    : Port(Direction_Input, name),
      m_pConnectedOutputPort(nullptr),
      m_defaultValue(defaultValue),
      m_pValue(&m_defaultValue)
{
}

float InputPort::value() const
{
    // NOTE: This is a clean way to access the connected output port.
    // However in order to improve the performance we keep the direct pointer
    // to the connected output port value or to this class' default value
    // if not connected.
    // return m_pConnectedOutputPort == nullptr ? m_defaultValue : m_pConnectedOutputPort->value();
    return *m_pValue;
}

void InputPort::update()
{
    if (m_pConnectedOutputPort != nullptr) {
        //m_pConnectedOutputPort->update();

        // Update audio unit directly bypassing the port chain.
        m_pConnectedOutputPort->updateAudioUnit();
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
    Q_ASSERT(pOutput != nullptr);
    m_pConnectedOutputPort = pOutput;
    m_pValue = pOutput->valuePtr();
}

void InputPort::disconnect()
{
    m_pConnectedOutputPort = nullptr;
    m_pValue = &m_defaultValue;
}
