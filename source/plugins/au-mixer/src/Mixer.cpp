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
#include <QtVariantProperty>
#include "Application.h"
#include "ISignalChain.h"
#include "Mixer.h"

/// Range of inputs number
const int Mixer::cMinNumberOfInputs(2);
const int Mixer::cMaxNumberOfInputs(32);

Mixer::Mixer(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pOutput = addOutput();
    m_mixFactor = 1.0f;
    createProperties();
}

Mixer::~Mixer()
{
}

void Mixer::createInputs(int nInputs)
{
    for (int i = 0; i < nInputs; ++i) {
        InputPort *pInput = addInput();
        m_inputs.append(pInput);
    }
}

void Mixer::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    data["nInputs"] = m_inputs.count();
    data["mixingFactor"] = m_pPropMixingFactor->value();
}

void Mixer::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    int nInputs = data.value("nInputs", 8).toInt(); // 8 - for backward compatibility to load old chains
    Q_ASSERT(nInputs >= cMinNumberOfInputs && nInputs <= cMaxNumberOfInputs);
    createInputs(nInputs);
    m_pPropMixingFactor->setValue(data.value("mixingFactor", 1.0));
}

void Mixer::processStart()
{
    m_mixFactor = m_pPropMixingFactor->value().toFloat();
}

void Mixer::processStop()
{
}

void Mixer::process()
{
    float sum = 0.0;
    for (const InputPort *pInput : m_inputs) {
        sum += pInput->getValue();
    }

    m_pOutput->setValue(sum * m_mixFactor);
}

void Mixer::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();

    m_pPropMixingFactor = propertyManager()->addProperty(QVariant::Double, "Mixing factor");
    m_pPropMixingFactor->setAttribute("minimum", 0.0);
    m_pPropMixingFactor->setAttribute("decimals", 3);
    m_pPropMixingFactor->setAttribute("singleStep", 0.1);
    m_pPropMixingFactor->setValue(1.0);

    pRoot->addSubProperty(m_pPropMixingFactor);

    QObject::connect (propertyManager(), &QtVariantPropertyManager::propertyChanged, [this](QtProperty *pProperty){
        m_mixFactor = m_pPropMixingFactor->value().toFloat();
    });

}
