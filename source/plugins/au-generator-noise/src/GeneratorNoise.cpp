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

#include <QtVariantPropertyManager>
#include <QtVariantProperty>
#include <qmath.h>
#include "Application.h"
#include "ISignalChain.h"
#include "GeneratorNoise.h"

GeneratorNoise::GeneratorNoise(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pOutput = addOutput();
}


void GeneratorNoise::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    AudioUnit::serialize(data, pContext);
}

void GeneratorNoise::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    AudioUnit::deserialize(data, pContext);
}

void GeneratorNoise::processStart()
{
}

void GeneratorNoise::processStop()
{
    reset();
}

void GeneratorNoise::process()
{
    float out = (float)(2.0 * rand()/(RAND_MAX + 1.0) - 1.0);
    m_pOutput->setValue(out);
}

void GeneratorNoise::reset()
{
    srand(0);
}
