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
#include <Flute.h>
#include "Application.h"
#include "ISignalChain.h"
#include "NoteOnEvent.h"
#include "NoteOffEvent.h"
#include "StkFlute.h"

const float cLowestFrequency(8.0);

void setCtrlPropertyAttrs(QtVariantProperty *pProp)
{
    Q_ASSERT(pProp != nullptr);
    pProp->setAttribute("minimum", 0.0);
    pProp->setAttribute("maximum", 1.0);
    pProp->setAttribute("singleStep", 0.1);
    pProp->setValue(0.5);
}

StkFlute::StkFlute(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pInputFreq = addInput("f");
    m_pInputVelocity = addInput("amp");
    m_pInputBreath = addInput("breath");

    m_pOutput = addOutput("out");

    m_note = -1;

    createProperties();

    m_pFlute = nullptr;
    try {
        m_pFlute = new stk::Flute(cLowestFrequency);
    } catch (stk::StkError err) {
        qCritical() << QString::fromStdString(err.getMessage());
    }
}

StkFlute::~StkFlute()
{
    delete m_pFlute;
}

void StkFlute::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    data["jetDelay"] = m_pPropJetDelay->value();
    data["noiseGain"] = m_pPropNoiseGain->value();
    AudioUnit::serialize(data, pContext);
}

void StkFlute::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    m_pPropJetDelay->setValue(data["jetDelay"]);
    m_pPropNoiseGain->setValue(data["noiseGain"]);
    AudioUnit::deserialize(data, pContext);
}

void StkFlute::processStart()
{
    if (m_pFlute != nullptr) {
        m_pFlute->setSampleRate(signalChain()->sampleRate());
    }
    m_note = -1;
}

void StkFlute::processStop()
{
    reset();
}

void StkFlute::process()
{
    if (m_pFlute == nullptr) {
        return;
    }

    float freq = m_pInputFreq->getValue();
    if (freq < cLowestFrequency) {
        return;
    }

    float breath = m_pInputBreath->getValue();

    m_pFlute->setFrequency(freq);
    m_pFlute->controlChange(Ctrl_BreathPressure, 128.0 * breath);
    m_pFlute->controlChange(Ctrl_JetDelay, 128.0 * m_pPropJetDelay->value().toDouble());
    m_pFlute->controlChange(Ctrl_NoiseGain, 128.0 * m_pPropNoiseGain->value().toDouble());

    float sample = m_pFlute->tick();

    m_pOutput->setValue(sample);
}

void StkFlute::reset()
{
    m_note = -1;
}

void StkFlute::noteOnEvent(NoteOnEvent *pEvent)
{
    Q_ASSERT(pEvent != nullptr);

    float f= pEvent->frequency();
    if (f > cLowestFrequency) {
        m_note = pEvent->noteNumber();
        m_pFlute->noteOn(f, pEvent->normalizedVelocity());
    }
}

void StkFlute::noteOffEvent(NoteOffEvent *pEvent)
{
    Q_ASSERT(pEvent != nullptr);

    if (pEvent->noteNumber() == m_note) {
        m_pFlute->noteOff(pEvent->normalizedVelocity());
    }
}

void StkFlute::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();

    m_pPropJetDelay = propertyManager()->addProperty(QVariant::Double, "Jet delay");
    setCtrlPropertyAttrs(m_pPropJetDelay);
    m_pPropNoiseGain = propertyManager()->addProperty(QVariant::Double, "Noise gain");
    setCtrlPropertyAttrs(m_pPropNoiseGain);

    pRoot->addSubProperty(m_pPropJetDelay);
    pRoot->addSubProperty(m_pPropNoiseGain);
}
