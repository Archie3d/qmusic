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
#include <QGraphicsPathItem>
#include <QGraphicsSimpleTextItem>
#include "Application.h"
#include "ISignalChain.h"
#include "Mixer.h"

const int cNumberOfInputs(8);

Mixer::Mixer(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    for (int i = 0; i < cNumberOfInputs; ++i) {
        InputPort *pInput = addInput();
        m_inputs.append(pInput);
    }
    m_pOutput = addOutput();
}

Mixer::~Mixer()
{
}

void Mixer::processStart()
{
}

void Mixer::processStop()
{
}

void Mixer::process()
{
    float sum = 0.0;
    foreach (const InputPort *pInput, m_inputs) {
        sum += pInput->value();
    }

    m_pOutput->setValue(sum);
}
