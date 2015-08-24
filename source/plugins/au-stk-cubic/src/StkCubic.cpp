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
#include <Cubic.h>
#include "Application.h"
#include "ISignalChain.h"
#include "NoteOnEvent.h"
#include "NoteOffEvent.h"
#include "StkCubic.h"

StkCubic::StkCubic(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pInput = addInput();
    m_pOutput = addOutput();

    createProperties();

    m_pCubic = new stk::Cubic();
}

StkCubic::~StkCubic()
{
    delete m_pCubic;
}

void StkCubic::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    data["a1"] = m_pPropA1->value();
    data["a2"] = m_pPropA2->value();
    data["a3"] = m_pPropA3->value();
    data["threshold"] = m_pPropThreshold->value();
    AudioUnit::serialize(data, pContext);
}

void StkCubic::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    m_pPropA1->setValue(data["a1"]);
    m_pPropA2->setValue(data["a2"]);
    m_pPropA3->setValue(data["a3"]);
    m_pPropThreshold->setValue(data["threshold"]);
    AudioUnit::deserialize(data, pContext);
}

void StkCubic::processStart()
{
    setValues();
}

void StkCubic::processStop()
{
    reset();
}

void StkCubic::process()
{
    float in = m_pInput->value();
    m_pOutput->setValue(m_pCubic->tick(in));
}

void StkCubic::reset()
{
    m_pOutput->setValue(0.0f);
}

void StkCubic::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();

    QtVariantProperty *pCoefs = propertyManager()->addProperty(propertyManager()->groupTypeId(), "Coefficients");

    m_pPropA1 = propertyManager()->addProperty(QVariant::Double, "A1");
    m_pPropA1->setAttribute("singleStep", 0.0);
    m_pPropA1->setValue(1.0);
    pCoefs->addSubProperty(m_pPropA1);

    m_pPropA2 = propertyManager()->addProperty(QVariant::Double, "A2");
    m_pPropA2->setAttribute("singleStep", 0.0);
    m_pPropA2->setValue(0.0);
    pCoefs->addSubProperty(m_pPropA2);

    m_pPropA3 = propertyManager()->addProperty(QVariant::Double, "A3");
    m_pPropA3->setAttribute("singleStep", 0.0);
    m_pPropA3->setValue(-1.0 / 3.0);
    pCoefs->addSubProperty(m_pPropA3);


    m_pPropThreshold = propertyManager()->addProperty(QVariant::Double, "Threshold");
    m_pPropThreshold->setAttribute("minimum", 0.0);
    m_pPropThreshold->setAttribute("maximum", 1.0);
    m_pPropThreshold->setAttribute("singleStep", 0.1);
    m_pPropThreshold->setValue(0.66);

    pRoot->addSubProperty(pCoefs);
    pRoot->addSubProperty(m_pPropThreshold);

    // Properties change handler
    QObject::connect (propertyManager(), &QtVariantPropertyManager::propertyChanged, [this](QtProperty *pProperty){
        Q_UNUSED(pProperty);
        setValues();
    });
}

void StkCubic::setValues()
{
    m_pCubic->setA1(m_pPropA1->value().toFloat());
    m_pCubic->setA2(m_pPropA2->value().toFloat());
    m_pCubic->setA3(m_pPropA3->value().toFloat());
    m_pCubic->setThreshold(m_pPropThreshold->value().toFloat());
}
