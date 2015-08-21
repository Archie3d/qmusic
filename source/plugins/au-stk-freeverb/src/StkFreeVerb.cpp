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
#include <FreeVerb.h>
#include "Application.h"
#include "ISignalChain.h"
#include "NoteOnEvent.h"
#include "NoteOffEvent.h"
#include "StkFreeVerb.h"

void setPropertyAttrs(QtVariantProperty *pProp, float min, float max, float step, float value)
{
    Q_ASSERT(pProp != nullptr);
    pProp->setAttribute("minimum", min);
    pProp->setAttribute("maximum", max);
    pProp->setAttribute("singleStep", step);
    pProp->setValue(value);
}

StkFreeVerb::StkFreeVerb(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pInputLeft = addInput("L");
    m_pInputRight = addInput("R");
    m_pOutputLeft = addOutput("L");
    m_pOutputRight = addOutput("R");

    createProperties();

    m_pFreeVerb = new stk::FreeVerb();
}

StkFreeVerb::~StkFreeVerb()
{
    delete m_pFreeVerb;
}

void StkFreeVerb::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    data["roomSize"] = m_pPropRoomSize->value();
    data["damping"] = m_pPropDamping->value();
    data["lrMix"] = m_pPropWidth->value();
    data["frozen"] = m_pPropFrozen->value();
    data["effectMix"] = m_pPropEffectMix->value();
    AudioUnit::serialize(data, pContext);
}

void StkFreeVerb::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    m_pPropRoomSize->setValue(data["roomSize"]);
    m_pPropDamping->setValue(data["damping"]);
    m_pPropWidth->setValue(data["lrMix"]);
    m_pPropFrozen->setValue(data["frozen"]);
    m_pPropEffectMix->setValue(data["effectMix"]);
    AudioUnit::deserialize(data, pContext);
}

void StkFreeVerb::processStart()
{
    m_pFreeVerb->clear();
    setValues();
}

void StkFreeVerb::processStop()
{
    reset();
}

void StkFreeVerb::process()
{
    float left = m_pInputLeft->value();
    float right = m_pInputRight->value();
    m_pOutputLeft->setValue(m_pFreeVerb->tick(left, right, 0));
    m_pOutputRight->setValue(m_pFreeVerb->tick(left, right, 1));
}

void StkFreeVerb::reset()
{
    m_pFreeVerb->clear();
    m_pOutputLeft->setValue(0.0f);
    m_pOutputRight->setValue(0.0f);
}

void StkFreeVerb::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();
    m_pPropRoomSize = propertyManager()->addProperty(QVariant::Double, "Room size");
    setPropertyAttrs(m_pPropRoomSize, 0.0f, 1.0f, 0.1f, 0.5f);

    m_pPropDamping = propertyManager()->addProperty(QVariant::Double, "Damping");
    setPropertyAttrs(m_pPropDamping, 0.0f, 1.0f, 0.1f, 0.4f);

    m_pPropWidth = propertyManager()->addProperty(QVariant::Double, "Left-right mix");
    setPropertyAttrs(m_pPropWidth, 0.0f, 1.0f, 0.1f, 1.0f);

    m_pPropFrozen = propertyManager()->addProperty(QVariant::Bool, "Frozen");
    m_pPropFrozen->setValue(false);

    m_pPropEffectMix = propertyManager()->addProperty(QVariant::Double, "Effect mix");
    setPropertyAttrs(m_pPropEffectMix, 0.0f, 1.0f, 0.1f, 0.75f);

    pRoot->addSubProperty(m_pPropRoomSize);
    pRoot->addSubProperty(m_pPropDamping);
    pRoot->addSubProperty(m_pPropWidth);
    pRoot->addSubProperty(m_pPropFrozen);
    pRoot->addSubProperty(m_pPropEffectMix);

    // Properties change handler
    QObject::connect (propertyManager(), &QtVariantPropertyManager::propertyChanged, [this](QtProperty *pProperty){
        Q_UNUSED(pProperty);
        setValues();
    });
}

void StkFreeVerb::setValues()
{
    m_pFreeVerb->setRoomSize(m_pPropRoomSize->value().toFloat());
    m_pFreeVerb->setDamping(m_pPropDamping->value().toFloat());
    m_pFreeVerb->setWidth(m_pPropWidth->value().toFloat());
    m_pFreeVerb->setMode(m_pPropFrozen->value().toBool());
    m_pFreeVerb->setEffectMix(m_pPropEffectMix->value().toFloat());
}
