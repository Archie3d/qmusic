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
#include <Saxofony.h>
#include "Application.h"
#include "ISignalChain.h"
#include "NoteOnEvent.h"
#include "NoteOffEvent.h"
#include "StkSaxofony.h"

const float cLowestFrequency(20.0);

void setCtrlPropertyAttrs(QtVariantProperty *pProp)
{
    Q_ASSERT(pProp != nullptr);
    pProp->setAttribute("minimum", 0.0);
    pProp->setAttribute("maximum", 1.0);
    pProp->setAttribute("singleStep", 0.1);
    pProp->setValue(0.5);
}

StkSaxofony::StkSaxofony(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pInputFreq = addInput("f");
    m_pInputVelocity = addInput("amp");
    m_pInputBreath = addInput("breath");

    m_pOutput = addOutput("out");

    m_note = -1;

    createProperties();

    m_pSaxofony = nullptr;
    try {
        m_pSaxofony = new stk::Saxofony(cLowestFrequency);
    } catch (stk::StkError err) {
        qCritical() << QString::fromStdString(err.getMessage());
    }
}

StkSaxofony::~StkSaxofony()
{
    delete m_pSaxofony;
}

void StkSaxofony::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    data["blowPosition"] = m_pPropBlowPosition->value();
    data["reedStiffness"] = m_pPropReedStiffness->value();
    data["reedAperture"] = m_pPropReedAperture->value();
    data["noiseGain"] = m_pPropNoiseGain->value();
    AudioUnit::serialize(data, pContext);
}

void StkSaxofony::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    m_pPropBlowPosition->setValue(data["blowPosition"]);
    m_pPropReedStiffness->setValue(data["reedStiffness"]);
    m_pPropReedAperture->setValue(data["reedAperture"]);
    m_pPropNoiseGain->setValue(data["noiseGain"]);
    AudioUnit::deserialize(data, pContext);
}

void StkSaxofony::processStart()
{
    if (m_pSaxofony != nullptr) {
        m_pSaxofony->setSampleRate(signalChain()->sampleRate());
    }
    m_note = -1;
}

void StkSaxofony::processStop()
{
    reset();
}

void StkSaxofony::process()
{
    if (m_pSaxofony == nullptr) {
        return;
    }

    float freq = m_pInputFreq->getValue();
    if (freq < cLowestFrequency) {
        return;
    }

    float breath = m_pInputBreath->getValue();

    m_pSaxofony->setFrequency(freq);

    m_pSaxofony->setBlowPosition(m_pPropBlowPosition->value().toDouble());
    m_pSaxofony->controlChange(Ctrl_ReedStiffness, 128.0 * m_pPropReedStiffness->value().toDouble());
    m_pSaxofony->controlChange(Ctrl_ReedAperture, 128.0 * m_pPropReedAperture->value().toDouble());
    m_pSaxofony->controlChange(Ctrl_NoiseGain, 128.0 * m_pPropNoiseGain->value().toDouble());
    m_pSaxofony->controlChange(Ctrl_BreathPressure, 128.0 * breath);

    float sample = m_pSaxofony->tick();

    m_pOutput->setValue(sample);
}

void StkSaxofony::reset()
{
    m_note = -1;
}

void StkSaxofony::noteOnEvent(NoteOnEvent *pEvent)
{
    Q_ASSERT(pEvent != nullptr);

    m_note = pEvent->noteNumber();
    m_pSaxofony->noteOn(pEvent->frequency(), pEvent->normalizedVelocity());
}

void StkSaxofony::noteOffEvent(NoteOffEvent *pEvent)
{
    Q_ASSERT(pEvent != nullptr);

    if (pEvent->noteNumber() == m_note) {
        m_pSaxofony->noteOff(pEvent->normalizedVelocity());
    }
}

void StkSaxofony::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();

    m_pPropBlowPosition = propertyManager()->addProperty(QVariant::Double, "Blow position");
    setCtrlPropertyAttrs(m_pPropBlowPosition);
    m_pPropReedStiffness = propertyManager()->addProperty(QVariant::Double, "Reed stiffness");
    setCtrlPropertyAttrs(m_pPropReedStiffness);
    m_pPropReedAperture = propertyManager()->addProperty(QVariant::Double, "Reed aperture");
    setCtrlPropertyAttrs(m_pPropReedAperture);
    m_pPropNoiseGain = propertyManager()->addProperty(QVariant::Double, "Noise gain");
    setCtrlPropertyAttrs(m_pPropNoiseGain);

    pRoot->addSubProperty(m_pPropBlowPosition);
    pRoot->addSubProperty(m_pPropReedStiffness);
    pRoot->addSubProperty(m_pPropReedAperture);
    pRoot->addSubProperty(m_pPropNoiseGain);
}
