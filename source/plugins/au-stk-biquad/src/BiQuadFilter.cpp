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
#include <QtVariantPropertyManager>
#include <QtVariantProperty>
#include <qmath.h>
#include "Application.h"
#include "ISignalChain.h"
#include "BiQuadFilter.h"


BiQuadFilter::BiQuadFilter(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin),
      m_filter()
{
    m_pInput = addInput("in");
    m_pInputCutOffFreq = addInput("f");

    m_pOutput = addOutput("out");

    createProperties();
}

BiQuadFilter::~BiQuadFilter()
{
}

void BiQuadFilter::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    data["filterType"] = m_pFilterType->value();
    data["filterRadius"] = m_pFilterRadius->value();
    AudioUnit::serialize(data, pContext);
}

void BiQuadFilter::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    m_pFilterType->setValue(data["filterTime"]);
    m_pFilterRadius->setValue(data["filterRadius"]);
    AudioUnit::deserialize(data, pContext);
}

void BiQuadFilter::processStart()
{
    m_filter.setSampleRate(signalChain()->sampleRate());
    setValues();
}

void BiQuadFilter::processStop()
{
}

void BiQuadFilter::process()
{
    float f = m_pInputCutOffFreq->value();
    if (m_f != f) {
        switch(m_filterType) {
        case Type_Resonance:
            m_filter.setResonance(f, m_radius, true);
            break;
        case Type_Notch:
            m_filter.setNotch(f, m_radius);
            break;
        default:
            break;
        }
        m_f = f;
    }
    m_pOutput->setValue(m_filter.tick(m_pInput->value()));
}

void BiQuadFilter::reset()
{
}

void BiQuadFilter::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();

    m_pFilterType = propertyManager()->addProperty(QtVariantPropertyManager::enumTypeId(), "Filter type");
    QVariantList list;
    list << "Resonance" << "Notch";
    m_pFilterType->setAttribute("enumNames", list);
    m_pFilterType->setValue(0);

    m_pFilterRadius = propertyManager()->addProperty(QVariant::Double, "Radius");
    m_pFilterRadius->setValue(0.5);
    m_pFilterRadius->setAttribute("singleStep", 0.1);

    pRoot->addSubProperty(m_pFilterType);
    pRoot->addSubProperty(m_pFilterRadius);

    // Properties change handler
    QObject::connect (propertyManager(), &QtVariantPropertyManager::propertyChanged, [this](QtProperty *pProperty){
        Q_UNUSED(pProperty);
        setValues();
    });

}

void BiQuadFilter::setValues()
{
    m_filterType = m_pFilterType->value().toInt() == 0 ? Type_Resonance : Type_Notch;
    m_radius = m_pFilterRadius->value().toFloat();
    m_f = 0.0f;
}
