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
#include <QDebug>
#include <QtVariantProperty>
#include <JCRev.h>
#include "Application.h"
#include "ISignalChain.h"
#include "NoteOnEvent.h"
#include "NoteOffEvent.h"
#include "StkJCRev.h"

StkJCRev::StkJCRev(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pInput = addInput("in");
    m_pOutputLeft = addOutput("L");
    m_pOutputRight = addOutput("R");

    createProperties();

    m_pJCRev = new stk::JCRev();
}

StkJCRev::~StkJCRev()
{
    delete m_pJCRev;
}

void StkJCRev::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    data["decayTime"] = m_pPropDecayTimeS->value();
    AudioUnit::serialize(data, pContext);
}

void StkJCRev::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    m_pPropDecayTimeS->setValue(data["decayTime"]);
    AudioUnit::deserialize(data, pContext);
}

void StkJCRev::processStart()
{
    m_pJCRev->clear();
    m_pJCRev->setSampleRate(signalChain()->sampleRate());
    m_pJCRev->setT60(m_pPropDecayTimeS->value().toFloat());
    m_pJCRev->setEffectMix(m_pPropEffectMix->value().toFloat());
}

void StkJCRev::processStop()
{
    reset();
}

void StkJCRev::process()
{
    float in = m_pInput->value();
    m_pOutputLeft->setValue(m_pJCRev->tick(in, 0));
    m_pOutputRight->setValue(m_pJCRev->tick(in, 1));
}

void StkJCRev::reset()
{
    m_pJCRev->clear();
}

void StkJCRev::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();
    m_pPropDecayTimeS = propertyManager()->addProperty(QVariant::Double, "Decay time, s");
    m_pPropDecayTimeS->setAttribute("minimum", 0.0);
    m_pPropDecayTimeS->setValue(1.0);

    m_pPropEffectMix = propertyManager()->addProperty(QVariant::Double, "Effect mix");
    m_pPropEffectMix->setAttribute("minimum", 0.0);
    m_pPropEffectMix->setAttribute("maximum", 1.0);
    m_pPropEffectMix->setAttribute("singleStep", 0.1);
    m_pPropEffectMix->setValue(0.5);

    pRoot->addSubProperty(m_pPropDecayTimeS);
    pRoot->addSubProperty(m_pPropEffectMix);

    // Properties change handler
    QObject::connect (propertyManager(), &QtVariantPropertyManager::propertyChanged, [this](QtProperty *pProperty){
        Q_UNUSED(pProperty);
        m_pJCRev->setT60(m_pPropDecayTimeS->value().toFloat());
        m_pJCRev->setEffectMix(m_pPropEffectMix->value().toFloat());
    });
}
