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
#include <Rhodey.h>
#include "Application.h"
#include "ISignalChain.h"
#include "NoteOnEvent.h"
#include "NoteOffEvent.h"
#include "StkRhodey.h"

StkRhodey::StkRhodey(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pInputFreq = addInput("f");
    m_pInputVelocity = addInput("amp");
    m_pInputNoteOn = addInput("on");

    m_pOutput = addOutput("out");

    m_note = -1;

    createProperties();

    QString rawPath = QApplication::applicationDirPath() + "/rawwaves";
    stk::Stk::setRawwavePath(rawPath.toStdString());
    m_pRhodey = nullptr;
    try {
        m_pRhodey = new stk::Rhodey();
    } catch (stk::StkError err) {
        qCritical() << QString::fromStdString(err.getMessage());
    }
}

StkRhodey::~StkRhodey()
{
    delete m_pRhodey;
}

void StkRhodey::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    AudioUnit::serialize(data, pContext);
}

void StkRhodey::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    AudioUnit::deserialize(data, pContext);
}

void StkRhodey::processStart()
{
    if (m_pRhodey != nullptr) {
        m_pRhodey->setSampleRate(signalChain()->sampleRate());
    }
    m_note = -1;
}

void StkRhodey::processStop()
{
    reset();
}

void StkRhodey::process()
{
    if (m_pRhodey == nullptr) {
        return;
    }

    float freq = m_pInputFreq->value();

    m_pRhodey->setFrequency(freq);

    float sample = m_pRhodey->tick();

    m_pOutput->setValue(sample);
}

void StkRhodey::reset()
{
    m_note = -1;
}

void StkRhodey::noteOnEvent(NoteOnEvent *pEvent)
{
    Q_ASSERT(pEvent != nullptr);

    m_note = pEvent->noteNumber();
    m_pRhodey->noteOn(pEvent->frequency(), pEvent->normalizedVelocity());
}

void StkRhodey::noteOffEvent(NoteOffEvent *pEvent)
{
    Q_ASSERT(pEvent != nullptr);

    if (pEvent->noteNumber() == m_note) {
        m_pRhodey->noteOff(pEvent->normalizedVelocity());
    }
}

void StkRhodey::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();
}
