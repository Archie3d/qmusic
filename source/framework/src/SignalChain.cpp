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

#include <QThread>
#include "Application.h"
#include "MidiInputDevice.h"
#include "AudioDevice.h"
#include "AudioUnit.h"
#include "SerializationContext.h"
#include "SignalChain.h"

const QString SignalChain::UID("SignalChain");

SignalChain::SignalChain()
    : m_timeStep(0.0),
      m_started(false),
      m_audioUnits()
{
}

SignalChain::~SignalChain()
{
    qDeleteAll(m_audioUnits);
}

void SignalChain::start()
{
    if (isStarted()) {
        return;
    }
    startAllAudioUnits();
    m_started = true;
}

void SignalChain::stop()
{
    if (!isStarted()) {
        return;
    }

    stopAllAudioUnits();
    m_started = false;
}

void SignalChain::reset()
{
    if (isStarted()) {
        // Cannot reset when running.
        return;
    }
    resetAllAudioUnits();
}

void SignalChain::addAudioUnit(IAudioUnit *pAudioUnit)
{
    Q_ASSERT(pAudioUnit != nullptr);
    if (m_audioUnits.contains(pAudioUnit)) {
        return;
    }

    AudioUnit *pAU = dynamic_cast<AudioUnit*>(pAudioUnit);
    if (pAU) {
        pAU->setSignalChain(this);
    }
    m_audioUnits.append(pAudioUnit);
}

void SignalChain::removeAudioUnit(IAudioUnit *pAudioUnit)
{
    Q_ASSERT(pAudioUnit != nullptr);
    AudioUnit *pAU = dynamic_cast<AudioUnit*>(pAudioUnit);
    if (pAU) {
        pAU->setSignalChain(nullptr);
    }
    m_audioUnits.removeOne(pAudioUnit);
}

void SignalChain::prepareUpdate()
{
    foreach (IAudioUnit *pAudioUnit, m_audioUnits) {
        pAudioUnit->prepareUpdate();
    }
}


void SignalChain::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);

    QVariantList units;
    foreach (IAudioUnit *pAu, m_audioUnits) {
        units.append(pContext->serialize(pAu));
    }
    data["audioUnits"] = units;
}

void SignalChain::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);

    // Delete all existing audio units
    qDeleteAll(m_audioUnits);

    QVariantList units = data["audioUnits"].toList();
    foreach (const QVariant &v, units) {
        ISerializable *pSer = pContext->deserialize(v);
        IAudioUnit *pAu = dynamic_cast<IAudioUnit*>(pSer);
        addAudioUnit(pAu);
    }
}

void SignalChain::startAllAudioUnits()
{
    foreach (IAudioUnit *pAudioUnit, m_audioUnits) {
        pAudioUnit->start();
    }
}

void SignalChain::stopAllAudioUnits()
{
    foreach (IAudioUnit *pAudioUnit, m_audioUnits) {
        pAudioUnit->stop();
    }
}

void SignalChain::resetAllAudioUnits()
{
    foreach (IAudioUnit *pAudioUnit, m_audioUnits) {
        pAudioUnit->reset();
    }
}
