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
#include <Clarinet.h>
#include "Application.h"
#include "ISignalChain.h"
#include "NoteOnEvent.h"
#include "NoteOffEvent.h"
#include "StkClarinet.h"

const float cLowestFrequency(8.0);

void setCtrlPropertyAttrs(QtVariantProperty *pProp)
{
    Q_ASSERT(pProp != nullptr);
    pProp->setAttribute("minimum", 0.0);
    pProp->setAttribute("maximum", 1.0);
    pProp->setAttribute("singleStep", 0.1);
    pProp->setValue(0.5);
}

StkClarinet::StkClarinet(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pInputFreq = addInput("f");
    m_pInputVelocity = addInput("amp");
    m_pInputBreath = addInput("breath");

    m_pOutput = addOutput("out");

    m_note = -1;

    createProperties();

    m_pClarinet = nullptr;
    try {
        m_pClarinet = new stk::Clarinet(cLowestFrequency);
    } catch (stk::StkError err) {
        qCritical() << QString::fromStdString(err.getMessage());
    }
}

StkClarinet::~StkClarinet()
{
    delete m_pClarinet;
}

void StkClarinet::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    data["reedStiffness"] = m_pPropReedStiffness->value();
    data["noiseGain"] = m_pPropNoiseGain->value();
    AudioUnit::serialize(data, pContext);
}

void StkClarinet::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    m_pPropReedStiffness->setValue(data["reedStiffness"]);
    m_pPropNoiseGain->setValue(data["noiseGain"]);
    AudioUnit::deserialize(data, pContext);
}

void StkClarinet::processStart()
{
    if (m_pClarinet != nullptr) {
        m_pClarinet->setSampleRate(signalChain()->sampleRate());
    }
    m_note = -1;
}

void StkClarinet::processStop()
{
    reset();
}

void StkClarinet::process()
{
    if (m_pClarinet == nullptr) {
        return;
    }


    float freq = m_pInputFreq->value();
    if (freq < cLowestFrequency) {
        return;
    }

    float breath = m_pInputBreath->value();

    m_pClarinet->setFrequency(freq);

    m_pClarinet->controlChange(Ctrl_ReedStiffness, 128.0 * m_pPropReedStiffness->value().toDouble());
    m_pClarinet->controlChange(Ctrl_NoiseGain, 128.0 * m_pPropNoiseGain->value().toDouble());
    m_pClarinet->controlChange(Ctrl_BreathPressure, 128.0 * breath);

    float sample = m_pClarinet->tick();

    m_pOutput->setValue(sample);
}

void StkClarinet::reset()
{
    m_note = -1;
}

void StkClarinet::noteOnEvent(NoteOnEvent *pEvent)
{
    Q_ASSERT(pEvent);

    float f = pEvent->frequency();
    if (f > cLowestFrequency) {
        m_note = pEvent->noteNumber();
        m_pClarinet->noteOn(f, pEvent->normalizedVelocity());
    }
}

void StkClarinet::noteOffEvent(NoteOffEvent *pEvent)
{
    Q_ASSERT(pEvent);

    if (pEvent->noteNumber() == m_note) {
        m_pClarinet->noteOff(pEvent->normalizedVelocity());
    }
}

void StkClarinet::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();

    m_pPropReedStiffness = propertyManager()->addProperty(QVariant::Double, "Reed stiffness");
    setCtrlPropertyAttrs(m_pPropReedStiffness);
    m_pPropNoiseGain = propertyManager()->addProperty(QVariant::Double, "Noise gain");
    setCtrlPropertyAttrs(m_pPropNoiseGain);

    pRoot->addSubProperty(m_pPropReedStiffness);
    pRoot->addSubProperty(m_pPropNoiseGain);
}
