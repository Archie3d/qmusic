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
#include "BiQuadFilterUnit.h"


BiQuadFilterUnit::BiQuadFilterUnit(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin),
      m_filter()
{
    m_pInput = addInput("in");
    m_pInputCutOffFreq = addInput("f");

    m_pOutput = addOutput("out");

    createProperties();
}

BiQuadFilterUnit::~BiQuadFilterUnit() = default;

void BiQuadFilterUnit::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    data["filterType"] = m_pFilterType->value();
    data["filterQ"] = m_pQFactor->value();
    data["filterDB"] = m_pDbGain->value();
    AudioUnit::serialize(data, pContext);
}

void BiQuadFilterUnit::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    m_pFilterType->setValue(data["filterType"]);
    m_pQFactor->setValue(data["filterQ"]);
    m_pDbGain->setValue(data["filterDB"]);
    AudioUnit::deserialize(data, pContext);
}

void BiQuadFilterUnit::processStart()
{
    m_filter.setSampleRate(signalChain()->sampleRate());
    setValues();
}

void BiQuadFilterUnit::processStop()
{
}

void BiQuadFilterUnit::process()
{
    float f = m_pInputCutOffFreq->getValue();
    if (m_f != f) {
        m_filter.setCutOffFrequency(f);
        m_f = f;
    }
    m_pOutput->setValue(m_filter.doFilter(m_pInput->getValue()));
}

void BiQuadFilterUnit::reset()
{
    m_filter.reset();
}

void BiQuadFilterUnit::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();

    m_pFilterType = propertyManager()->addProperty(QtVariantPropertyManager::enumTypeId(), "Filter type");
    QVariantList list;
    list << "Low-pass" << "High-pass" << "Band-pass" << "Notch" << "All-pass" << "Peaking EQ" << "Low-shelf" << "High-shelf";
    m_pFilterType->setAttribute("enumNames", list);
    m_pFilterType->setValue(0);

    m_pQFactor = propertyManager()->addProperty(QVariant::Double, "Q-factor");
    m_pQFactor->setValue(0.7);
    m_pQFactor->setAttribute("singleStep", 0.1);

    m_pDbGain = propertyManager()->addProperty(QVariant::Double, "dB gain");
    m_pDbGain->setValue(1.0);
    m_pDbGain->setAttribute("singleStep", 0.1);

    pRoot->addSubProperty(m_pFilterType);
    pRoot->addSubProperty(m_pQFactor);
    pRoot->addSubProperty(m_pDbGain);

    // Properties change handler
    QObject::connect (propertyManager(), &QtVariantPropertyManager::propertyChanged, [this](QtProperty *pProperty){
        Q_UNUSED(pProperty);
        setValues();
    });

}

void BiQuadFilterUnit::setValues()
{
    const QVector<BiquadFilter::Type> cType {
        BiquadFilter::Type_LPF,
        BiquadFilter::Type_HPF,
        BiquadFilter::Type_BPF,
        BiquadFilter::Type_Notch,
        BiquadFilter::Type_APF,
        BiquadFilter::Type_PeakingEQ,
        BiquadFilter::Type_LowShelf,
        BiquadFilter::Type_HighShelf
    };

    m_filter.setType(cType.at(m_pFilterType->value().toInt()));
    m_filter.setQFactor(m_pQFactor->value().toFloat());
    m_filter.setDBGain(m_pDbGain->value().toFloat());
}
