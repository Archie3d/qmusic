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
#include "LHPFilter.h"


LHPFilter::LHPFilter(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin),
      m_filter()
{
    m_pInput = addInput("in");
    m_pInputCutOffFreq = addInput("f");

    m_pOutput = addOutput("out");

    createProperties();
}

LHPFilter::~LHPFilter()
{
}

void LHPFilter::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    data["filterType"] = m_pFilterType->value();
    AudioUnit::serialize(data, pContext);
}

void LHPFilter::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    m_pFilterType->setValue(data["filterType"]);
    AudioUnit::deserialize(data, pContext);
}

void LHPFilter::processStart()
{
    m_filter.setSampleRate(signalChain()->sampleRate());
    m_filter.reset();
}

void LHPFilter::processStop()
{
}

void LHPFilter::process()
{
    m_filter.setCutOffFrequency(m_pInputCutOffFreq->getValue());
    m_pOutput->setValue(m_filter.doFilter(m_pInput->getValue()));
}

void LHPFilter::reset()
{
    m_filter.reset();
}

void LHPFilter::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();

    m_pFilterType = propertyManager()->addProperty(QtVariantPropertyManager::enumTypeId(), "Filter type");
    QVariantList list;
    list << "Low-pass" << "High-pass";
    m_pFilterType->setAttribute("enumNames", list);
    m_pFilterType->setValue(0);
    pRoot->addSubProperty(m_pFilterType);

    QObject::connect(propertyManager(), &QtVariantPropertyManager::propertyChanged, [this](QtProperty *pProperty){
        Q_UNUSED(pProperty);
        setValues();
    });
}

void LHPFilter::setValues()
{
    int type = m_pFilterType->value().toInt();
    m_filter.setType(type == 0 ? VAOnePoleFilter::Type_LP : VAOnePoleFilter::Type_HP);
}
