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
#include "NoteOnEvent.h"
#include "NoteOffEvent.h"
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
    m_pInputFreq = addInput("f");
    m_pInputVelocity = addInput("amp");

    m_pOutput = addOutput("out");

    createProperties();

    m_pGuitar = new stk::Guitar();
}

StkGuitar::~StkGuitar()
{
    delete m_pGuitar;
}

void StkGuitar::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    data["pickPosition"] = m_pPropPickPosition->value();
    data["stringDamping"] = m_pPropStringDamping->value();
    AudioUnit::serialize(data, pContext);
}

void StkGuitar::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    m_pPropPickPosition->setValue(data["pickPosition"]);
    m_pPropStringDamping->setValue(data["stringDamping"]);
    AudioUnit::deserialize(data, pContext);
}

void StkGuitar::processStart()
{
    m_pGuitar->setSampleRate(signalChain()->sampleRate());
    m_note = -1;
    setValues();
}

void StkGuitar::processStop()
{
    reset();
}

void StkGuitar::process()
{
    float freq = m_pInputFreq->value();

    if (freq < cMinFrequency) {
        return;
    }

    m_pGuitar->setFrequency(freq);
    float sample = m_pGuitar->tick();
    m_pOutput->setValue(sample);
}

void StkGuitar::reset()
{
    m_note = -1;
    m_pOutput->setValue(0.0f);
}

void StkGuitar::noteOnEvent(NoteOnEvent *pEvent)
{
    Q_ASSERT(pEvent != nullptr);

    float f = pEvent->frequency();

    if (f > cMinFrequency) {
        m_note = pEvent->noteNumber();
        m_pGuitar->noteOn(f, pEvent->normalizedVelocity());
    }
}

void StkGuitar::noteOffEvent(NoteOffEvent *pEvent)
{
    Q_ASSERT(pEvent != nullptr);

    if (pEvent->noteNumber() == m_note) {
        m_pGuitar->noteOff(pEvent->normalizedVelocity());
    }
}

void StkGuitar::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();

    m_pPropPickPosition = propertyManager()->addProperty(QVariant::Double, "Pick position");
    setCtrlPropertyAttrs(m_pPropPickPosition, 0.5, 0.01, 1.0);

    m_pPropStringDamping = propertyManager()->addProperty(QVariant::Double, "String damping");
    setCtrlPropertyAttrs(m_pPropStringDamping, 0.5, 0.0, 0.99);

    pRoot->addSubProperty(m_pPropPickPosition);
    pRoot->addSubProperty(m_pPropStringDamping);

    // Properties change handler
    QObject::connect (propertyManager(), &QtVariantPropertyManager::propertyChanged, [this](QtProperty *pProperty){
        Q_UNUSED(pProperty);
        setValues();
    });
}

void StkGuitar::setValues()
{
    m_pGuitar->controlChange(Ctrl_PickPosition, 128.0 * m_pPropPickPosition->value().toDouble());
    m_pGuitar->controlChange(Ctrl_StringDamping, 128.0 * m_pPropStringDamping->value().toDouble());
}
