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
#include "AudioDevicesManager.h"
#include "MidiNote.h"
#include "MidiDevice.h"
#include "MidiInputDevice.h"
#include "ISignalChain.h"
#include "SignalChainEvent.h"
#include "MidiIn.h"

const QColor cDefaultColor(230, 240, 210);

MidiIn::MidiIn(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pOutputFreq = addOutput("f");
    m_pOutputVelocity = addOutput("velocity");

    createProperties();
}

MidiIn::~MidiIn()
{
}

void MidiIn::handleEvent(SignalChainEvent *pEvent)
{
    Q_ASSERT(pEvent != nullptr);

    // Handle noteOn, noteOff, and pitchBend events
    QString name = pEvent->name();
    QVariantMap map = pEvent->data().toMap();
    if (name == "noteOn") {
        m_noteNumber = map["number"].toInt();
        m_frequency = MidiNote(m_noteNumber).frequency();
        m_velocity = map["velocity"].toFloat() / 127.0f;
    } else if (name == "noteOff") {
        int number = map["number"].toInt();
        if (m_noteNumber == number) {
            m_velocity = map["velocity"].toFloat() / 127.0;
        }
    } else if (name == "pitchBend") {
        int bend = map["value"].toInt() - 8192;
        float dBend = float(bend) / 8192.0f;
        dBend *= m_pPropPitchBendSemitones->value().toFloat();
        m_frequencyBend = pow(2.0, dBend / 12.0);
    }
}

QColor MidiIn::color() const
{
    return cDefaultColor;
}

void MidiIn::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    data["pitchBendSemitones"] = m_pPropPitchBendSemitones->value();
    AudioUnit::serialize(data, pContext);
}

void MidiIn::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    AudioUnit::deserialize(data, pContext);
    m_pPropPitchBendSemitones->setValue(data["pitchBendSemitones"]);
}

void MidiIn::processStart()
{
    m_frequency = 0.0f;
    m_frequencyBend = 1.0f;
    m_velocity = 0.0f;
}

void MidiIn::processStop()
{
}

void MidiIn::process()
{
    m_pOutputFreq->setValue(m_frequency * m_frequencyBend);
    m_pOutputVelocity->setValue(m_velocity);
}

void MidiIn::reset()
{
}

void MidiIn::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();

    QtVariantProperty *pPropPitchBend = propertyManager()->addProperty(propertyManager()->groupTypeId(), "Pitch bend");

    m_pPropPitchBendSemitones = propertyManager()->addProperty(QVariant::Int, "Semitones");
    m_pPropPitchBendSemitones->setAttribute("minimum", 1);
    m_pPropPitchBendSemitones->setAttribute("maximum", 24);
    m_pPropPitchBendSemitones->setValue(1);

    pPropPitchBend->addSubProperty(m_pPropPitchBendSemitones);
    pRoot->addSubProperty(pPropPitchBend);
}
