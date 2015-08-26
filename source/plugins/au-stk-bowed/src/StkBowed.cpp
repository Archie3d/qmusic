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
#include <Bowed.h>
#include "Application.h"
#include "ISignalChain.h"
#include "NoteOnEvent.h"
#include "NoteOffEvent.h"
#include "StkBowed.h"

const float cLowestFrequency(8.0);

void setCtrlPropertyAttrs(QtVariantProperty *pProp)
{
    Q_ASSERT(pProp != nullptr);
    pProp->setAttribute("minimum", 0.0);
    pProp->setAttribute("maximum", 1.0);
    pProp->setAttribute("singleStep", 0.1);
    pProp->setValue(0.5);
}

StkBowed::StkBowed(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pInputFreq = addInput("f");
    m_pInputVelocity = addInput("amp");

    m_pOutput = addOutput("out");

    m_note = -1;

    createProperties();

    m_pBowed = nullptr;
    try {
        m_pBowed = new stk::Bowed(cLowestFrequency);
    } catch (stk::StkError err) {
        qCritical() << QString::fromStdString(err.getMessage());
    }
}

StkBowed::~StkBowed()
{
    delete m_pBowed;
}

void StkBowed::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    data["bowPressure"] = m_pPropBowPressure->value();
    data["bowPosition"] = m_pPropBowPosition->value();
    data["vibratoFrequency"] = m_pPropVibratoFrequency->value();
    data["vibratoGain"] = m_pPropVibratoGain->value();
    AudioUnit::serialize(data, pContext);
}

void StkBowed::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    m_pPropBowPressure->setValue(data["bowPressure"]);
    m_pPropBowPosition->setValue(data["bowPosition"]);
    m_pPropVibratoFrequency->setValue(data["vibratoFrequency"]);
    m_pPropVibratoGain->setValue(data["vibratoGain"]);
    AudioUnit::deserialize(data, pContext);
}

void StkBowed::processStart()
{
    if (m_pBowed != nullptr) {
        m_pBowed->setSampleRate(signalChain()->sampleRate());
        setProperties();
    }
    m_note = -1;
}

void StkBowed::processStop()
{
    reset();
}

void StkBowed::process()
{
    if (m_pBowed == nullptr) {
        return;
    }

    float freq = m_pInputFreq->value();
    if (freq < cLowestFrequency) {
        return;
    }

    float amp = m_pInputVelocity->value();

    m_pBowed->setFrequency(freq);
    // TODO: Should it be Ctrl_BowVelocity instead?
    m_pBowed->controlChange(Ctrl_Volume, 128.0 * amp);

    float sample = m_pBowed->tick();

    m_pOutput->setValue(sample);
}

void StkBowed::reset()
{
    m_note = -1;
}

void StkBowed::noteOnEvent(NoteOnEvent *pEvent)
{
    Q_ASSERT(pEvent != nullptr);

    float f= pEvent->frequency();
    if (f > cLowestFrequency) {
        m_note = pEvent->noteNumber();
        m_pBowed->noteOn(f, pEvent->normalizedVelocity());
    }
}

void StkBowed::noteOffEvent(NoteOffEvent *pEvent)
{
    Q_ASSERT(pEvent != nullptr);

    if (pEvent->noteNumber() == m_note) {
        m_pBowed->noteOff(pEvent->normalizedVelocity());
    }
}

void StkBowed::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();

    m_pPropBowPressure = propertyManager()->addProperty(QVariant::Double, "Bow pressure");
    setCtrlPropertyAttrs(m_pPropBowPressure);
    m_pPropBowPosition = propertyManager()->addProperty(QVariant::Double, "Bow position");
    setCtrlPropertyAttrs(m_pPropBowPosition);

    QtVariantProperty *pVibrato = propertyManager()->addProperty(propertyManager()->groupTypeId(), "Vibrato");
    m_pPropVibratoFrequency = propertyManager()->addProperty(QVariant::Double, "Frequency");
    setCtrlPropertyAttrs(m_pPropVibratoFrequency);
    m_pPropVibratoGain = propertyManager()->addProperty(QVariant::Double, "Gain");
    setCtrlPropertyAttrs(m_pPropVibratoGain);
    pVibrato->addSubProperty(m_pPropVibratoFrequency);
    pVibrato->addSubProperty(m_pPropVibratoGain);

    pRoot->addSubProperty(m_pPropBowPressure);
    pRoot->addSubProperty(m_pPropBowPosition);
    pRoot->addSubProperty(pVibrato);

    // Properties change handler
    QObject::connect (propertyManager(), &QtVariantPropertyManager::propertyChanged, [this](QtProperty *pProperty){
        Q_UNUSED(pProperty);
        setProperties();
    });
}

void StkBowed::setProperties()
{
    if (m_pBowed != nullptr) {
        m_pBowed->controlChange(Ctrl_BowPressure, 128.0 * m_pPropBowPressure->value().toFloat());
        m_pBowed->controlChange(Ctrl_BowPosition, 128.0 * m_pPropBowPosition->value().toFloat());
        m_pBowed->controlChange(Ctrl_VibratoFrequency, 128.0 * m_pPropVibratoFrequency->value().toFloat());
        m_pBowed->controlChange(Ctrl_VibratoGain, 128.0 * m_pPropVibratoGain->value().toFloat());
    }
}
