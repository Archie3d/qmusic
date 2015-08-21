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
#include <QDebug>
#include <QtVariantProperty>
#include <BeeThree.h>
#include "Application.h"
#include "ISignalChain.h"
#include "NoteOnEvent.h"
#include "NoteOffEvent.h"
#include "StkBeeThree.h"

const float cMinFrequency(8.0f);

void setCtrlPropertyAttrs(QtVariantProperty *pProp)
{
    Q_ASSERT(pProp != nullptr);
    pProp->setAttribute("minimum", 0.0);
    pProp->setAttribute("maximum", 1.0);
    pProp->setAttribute("singleStep", 0.1);
    pProp->setValue(0.5);
}

StkBeeThree::StkBeeThree(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pInputFreq = addInput("f");
    m_pInputVelocity = addInput("amp");
    m_pOutput = addOutput("out");

    createProperties();

    QString rawPath = QApplication::applicationDirPath() + "/rawwaves";
    stk::Stk::setRawwavePath(rawPath.toStdString());
    m_pBeeThree = nullptr;
    try {
        m_pBeeThree = new stk::BeeThree();
    } catch (stk::StkError err) {
        qCritical() << QString::fromStdString(err.getMessage());
    }
}

StkBeeThree::~StkBeeThree()
{
    delete m_pBeeThree;
}

void StkBeeThree::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    data["operator4"] = m_pPropOperator4->value();
    data["operator3"] = m_pPropOperator3->value();
    data["lfoSpeed"] = m_pPropLFOSpeed->value();
    data["lfoDepth"] = m_pPropLFODepth->value();
    AudioUnit::serialize(data, pContext);
}

void StkBeeThree::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    m_pPropOperator4->setValue(data["operator4"]);
    m_pPropOperator3->setValue(data["operator3"]);
    m_pPropLFOSpeed->setValue(data["lfoSpeed"]);
    m_pPropLFODepth->setValue(data["lfoDepth"]);
    AudioUnit::deserialize(data, pContext);
}

void StkBeeThree::processStart()
{
    if (m_pBeeThree != nullptr) {
        m_pBeeThree->setSampleRate(signalChain()->sampleRate());
    }
    m_note = -1;
}

void StkBeeThree::processStop()
{
    reset();
}

void StkBeeThree::process()
{
    if (m_pBeeThree == nullptr) {
        return;
    }

    float freq = m_pInputFreq->value();

    if (freq < cMinFrequency) {
        return;
    }

    m_pBeeThree->controlChange(Ctrl_Operator4, 128.0 * m_pPropOperator4->value().toDouble());
    m_pBeeThree->controlChange(Ctrl_Operator3, 128.0 * m_pPropOperator3->value().toDouble());
    m_pBeeThree->controlChange(Ctrl_LFOSpeed, 128.0 * m_pPropLFOSpeed->value().toDouble());
    m_pBeeThree->controlChange(Ctrl_LFODepth, 128.0 * m_pPropLFODepth->value().toDouble());

    m_pBeeThree->setFrequency(freq);

    float sample = m_pBeeThree->tick();

    m_pOutput->setValue(sample);
}

void StkBeeThree::reset()
{
}

void StkBeeThree::noteOnEvent(NoteOnEvent *pEvent)
{
    Q_ASSERT(pEvent != nullptr);

    float f = pEvent->frequency();
    if (f > cMinFrequency) {
        m_note = pEvent->noteNumber();
        m_pBeeThree->noteOn(f, pEvent->normalizedVelocity());
    }
}

void StkBeeThree::noteOffEvent(NoteOffEvent *pEvent)
{
    Q_ASSERT(pEvent != nullptr);

    if (pEvent->noteNumber() == m_note) {
        m_pBeeThree->noteOff(pEvent->normalizedVelocity());
    }

}

void StkBeeThree::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();

    m_pPropOperator4 = propertyManager()->addProperty(QVariant::Double, "Feedback gain");
    setCtrlPropertyAttrs(m_pPropOperator4);
    m_pPropOperator3 = propertyManager()->addProperty(QVariant::Double, "Gain");
    setCtrlPropertyAttrs(m_pPropOperator3);
    m_pPropLFOSpeed = propertyManager()->addProperty(QVariant::Double, "LFO Speed");
    setCtrlPropertyAttrs(m_pPropLFOSpeed);
    m_pPropLFODepth = propertyManager()->addProperty(QVariant::Double, "LFO depth");
    setCtrlPropertyAttrs(m_pPropLFODepth);

    pRoot->addSubProperty(m_pPropOperator4);
    pRoot->addSubProperty(m_pPropOperator3);
    pRoot->addSubProperty(m_pPropLFOSpeed);
    pRoot->addSubProperty(m_pPropLFODepth);
}
