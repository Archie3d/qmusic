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
#include <Guitar.h>
#include "Application.h"
#include "ISignalChain.h"
#include "SignalChainEvent.h"
#include "StkGuitar.h"

const float cMinFrequency(50.0f);

void setCtrlPropertyAttrs(QtVariantProperty *pProp, double v = 0.5, double min = 0.0, double max = 1.0)
{
    Q_ASSERT(pProp != nullptr);
    pProp->setAttribute("minimum", min);
    pProp->setAttribute("maximum", max);
    pProp->setAttribute("singleStep", 0.1);
    pProp->setValue(v);
}

StkGuitar::StkGuitar(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pInputFreq = addInput("f", Signal::Type_Float);
    m_pInputVelocity = addInput("amp", Signal::Type_Float);

    m_pOutput = addOutput("out", Signal::Type_Float);

    createProperties();

    m_pGuitar = new stk::Guitar();
}

StkGuitar::~StkGuitar()
{
    delete m_pGuitar;
}

void StkGuitar::handleEvent(SignalChainEvent *pEvent)
{
    QString name = pEvent->name();

    float freq = m_pInputFreq->value().asFloat;
    float amp = m_pInputVelocity->value().asFloat;

    if (name == "noteOn") {
        if (freq > cMinFrequency) {
            m_note = pEvent->data().toMap()["number"].toInt();
            m_pGuitar->noteOn(freq, amp);
        }
    } else if (name == "noteOff") {
        int note = pEvent->data().toMap()["number"].toInt();
        if (note == m_note) {
            m_pGuitar->noteOff(amp);
        }
    }
}

void StkGuitar::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    data["pickPosition"] = m_pPropPickPosition->value();
    data["stringDumping"] = m_pPropStringDumping->value();
    AudioUnit::serialize(data, pContext);
}

void StkGuitar::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    m_pPropPickPosition->setValue(data["pickPosition"]);
    m_pPropStringDumping->setValue(data["stringDumping"]);
    AudioUnit::deserialize(data, pContext);
}

void StkGuitar::processStart()
{
    m_pGuitar->setSampleRate(signalChain()->sampleRate());
    m_note = -1;
}

void StkGuitar::processStop()
{
    reset();
}

void StkGuitar::process()
{
    float freq = m_pInputFreq->value().asFloat;
    float amp = m_pInputVelocity->value().asFloat;

    if (freq < cMinFrequency) {
        return;
    }

    m_pGuitar->controlChange(Ctrl_PickPosition, 128.0 * m_pPropPickPosition->value().toDouble());
    m_pGuitar->controlChange(Ctrl_StringDumping, 128.0 * m_pPropStringDumping->value().toDouble());

    m_pGuitar->setFrequency(freq);

    float sample = m_pGuitar->tick();

    m_pOutput->setFloatValue(sample);
}

void StkGuitar::reset()
{
    m_note = -1;
}

void StkGuitar::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();

    m_pPropPickPosition = propertyManager()->addProperty(QVariant::Double, "Pick position");
    setCtrlPropertyAttrs(m_pPropPickPosition, 0.5, 0.01, 1.0);

    m_pPropStringDumping = propertyManager()->addProperty(QVariant::Double, "String dumping");
    setCtrlPropertyAttrs(m_pPropStringDumping, 0.5, 0.0, 0.99);


    pRoot->addSubProperty(m_pPropPickPosition);
    pRoot->addSubProperty(m_pPropStringDumping);
}
