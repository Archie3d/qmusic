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
#include "NoteOnEvent.h"
#include "NoteOffEvent.h"
#include "PitchBendEvent.h"
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

QColor MidiIn::color() const
{
    return cDefaultColor;
}

int MidiIn::flags() const
{
     return Flag_NoTitle
             | Flag_NoFrame;

}

QGraphicsItem* MidiIn::graphicsItem()
{
    QGraphicsPixmapItem *pItem = new QGraphicsPixmapItem(QPixmap::fromImage(QImage(":/au-midi-in/piano_48.png")));
    return pItem;
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
    m_semitones = m_pPropPitchBendSemitones->value().toFloat();
}

void MidiIn::processStop()
{
}

void MidiIn::process()
{
    m_pOutputFreq->setValue(m_frequency * m_frequencyBend);

    // Perform some filtering on velocity value to avoid glitches
    float f = 0.8f;
    m_pOutputVelocity->setValue(m_pOutputVelocity->value() * f +
                                m_velocity * (1.0f - f));
}

void MidiIn::reset()
{
}

void MidiIn::noteOnEvent(NoteOnEvent *pEvent)
{
    m_noteNumber = pEvent->noteNumber();
    m_frequency = MidiNote(m_noteNumber).frequency();
    m_velocity = pEvent->normalizedVelocity();
}

void MidiIn::noteOffEvent(NoteOffEvent *pEvent)
{
    if (pEvent->noteNumber() == m_noteNumber) {
        m_velocity = pEvent->normalizedVelocity();
    }
}

void MidiIn::pitchBendEvent(PitchBendEvent *pEvent)
{
    m_frequencyBend = pEvent->bendFactor(m_semitones);
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
