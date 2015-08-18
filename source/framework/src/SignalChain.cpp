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
#include "SignalChainEvent.h"
#include "SignalChain.h"

// Event process period in samples
#define EVENTS_PROCESS_PERIOD  (50)

const QString SignalChain::UID("SignalChain");

SignalChain::SignalChain()
    : m_timeStep(0.0),
      m_started(false),
      m_enabled(false),
      m_updateEventsCounter(0),
      m_audioUnits(),
      m_events()
{
}

SignalChain::~SignalChain()
{
    clearEventsQueue();
    qDeleteAll(m_audioUnits);
}

void SignalChain::start()
{
    if (isStarted()) {
        return;
    }
    m_updateEventsCounter = 0;
    startAllAudioUnits();
    m_enabled = false;
    m_started = true;
}

void SignalChain::stop()
{
    if (!isStarted()) {
        return;
    }

    clearEventsQueue();
    stopAllAudioUnits();
    m_started = false;
}

void SignalChain::reset()
{
    if (isStarted()) {
        // Cannot reset when running.
        return;
    }

    clearEventsQueue();
    resetAllAudioUnits();
    m_enabled = false;
}

void SignalChain::enable(bool v)
{
    m_enabled = v;
}

void SignalChain::postEvent(SignalChainEvent *pEvent)
{
    Q_ASSERT(pEvent != nullptr);

    if (isStarted()) {
        m_events.append(pEvent);
    }
}

void SignalChain::processEvents()
{
    QList<SignalChainEvent*> events = m_events;
    m_events.clear();
    foreach (SignalChainEvent *pEvent, events) {
        processEvent(pEvent);
        delete pEvent;
    }
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
    // Decimate the event processing occurrence in order to
    // decrease overhead of events handling for every sample.
    if ((m_updateEventsCounter++) % EVENTS_PROCESS_PERIOD == 0) {
        processEvents();
    }

    // Prepare audio units update
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

void SignalChain::clearEventsQueue()
{
    qDeleteAll(m_events);
    m_events.clear();
}

void SignalChain::processEvent(SignalChainEvent *pEvent)
{
    Q_ASSERT(pEvent != nullptr);
    foreach (IAudioUnit *pAudioUnit, m_audioUnits) {
        pAudioUnit->handleEvent(pEvent);
    }
}
