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

OutputPort::OutputPort()
    : Port(Direction_Output),
      m_value()
{
}

OutputPort::OutputPort(const QString &name)
    : Port(Direction_Output, name),
      m_value()
{
}

void OutputPort::update()
{
    updateAudioUnit();
}

int OutputPort::index() const
{
    AudioUnit *pAu = static_cast<AudioUnit*>(audioUnit());

    const QList<OutputPort*> &outputs = pAu->outputs();
    for (int i = 0; i < outputs.count(); ++i) {
        if (outputs[i] == this) {
            return i;
        }
    }
    return -1;
}
