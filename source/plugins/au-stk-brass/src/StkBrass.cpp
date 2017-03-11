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
#include <Brass.h>
#include "Application.h"
#include "ISignalChain.h"
#include "NoteOnEvent.h"
#include "NoteOffEvent.h"
#include "StkBrass.h"

const float cLowestFrequency(50.0);

void setCtrlPropertyAttrs(QtVariantProperty *pProp)
{
    Q_ASSERT(pProp != nullptr);
    pProp->setAttribute("minimum", 0.0);
    pProp->setAttribute("maximum", 1.0);
    pProp->setAttribute("singleStep", 0.1);
    pProp->setValue(0.5);
}

StkBrass::StkBrass(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pInputFreq = addInput("f");
    m_pInputVelocity = addInput("amp");
    m_pInputBreath = addInput("breath");

    m_pOutput = addOutput("out");

    m_note = -1;

    createProperties();

    m_pBrass = nullptr;
    try {
        m_pBrass = new stk::Brass(cLowestFrequency);
    } catch (stk::StkError err) {
        qCritical() << QString::fromStdString(err.getMessage());
    }
}

StkBrass::~StkBrass()
{
    delete m_pBrass;
}

void StkBrass::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    data["lipTension"] = m_pPropLipTension->value();
    data["slideLength"] = m_pPropSlideLength->value();
    AudioUnit::serialize(data, pContext);
}

void StkBrass::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    m_pPropLipTension->setValue(data["lipTension"]);
    m_pPropSlideLength->setValue(data["slideLength"]);
    AudioUnit::deserialize(data, pContext);
}

void StkBrass::processStart()
{
    if (m_pBrass != nullptr) {
        m_pBrass->setSampleRate(signalChain()->sampleRate());
    }
    m_note = -1;
}

void StkBrass::processStop()
{
    reset();
}

void StkBrass::process()
{
    if (m_pBrass == nullptr) {
        return;
    }

    float freq = m_pInputFreq->getValue();
    if (freq < cLowestFrequency) {
        return;
    }
    float breath = m_pInputBreath->getValue();

    m_pBrass->controlChange(Ctrl_LipTension, 128.0 * m_pPropLipTension->value().toDouble());
    m_pBrass->controlChange(Ctrl_SlideLength, 128.0 * m_pPropSlideLength->value().toDouble());
    m_pBrass->controlChange(Ctrl_BreathPressure, 128.0 * breath);

    m_pBrass->setFrequency(freq);

    float sample = m_pBrass->tick();

    m_pOutput->setValue(sample);
}

void StkBrass::reset()
{
    m_note = -1;
}

void StkBrass::noteOnEvent(NoteOnEvent *pEvent)
{
    Q_ASSERT(pEvent != nullptr);

    float f = pEvent->frequency();
    if (f > cLowestFrequency) {
        m_note = pEvent->noteNumber();
        m_pBrass->noteOn(f, pEvent->normalizedVelocity());
    }
}

void StkBrass::noteOffEvent(NoteOffEvent *pEvent)
{
    Q_ASSERT(pEvent != nullptr);

    if (pEvent->noteNumber() == m_note) {
        m_pBrass->noteOff(pEvent->normalizedVelocity());
    }
}

void StkBrass::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();

    m_pPropLipTension = propertyManager()->addProperty(QVariant::Double, "Lip tension");
    setCtrlPropertyAttrs(m_pPropLipTension);
    m_pPropSlideLength = propertyManager()->addProperty(QVariant::Double, "Slide length");
    setCtrlPropertyAttrs(m_pPropSlideLength);

    pRoot->addSubProperty(m_pPropLipTension);
    pRoot->addSubProperty(m_pPropSlideLength);
}
