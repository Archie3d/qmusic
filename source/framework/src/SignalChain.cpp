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
#include "AudioUnitPlugin.h"
#include "SerializationContext.h"
#include "SignalChainFactory.h"
#include "SignalChainEvent.h"
#include "SignalChain.h"

const QString SignalChain::UID("SignalChain");

SignalChain::SignalChain()
    : m_timeStep(0.0),
      m_started(false),
      m_enabled(false),
      m_updateEventsCounter(0),
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
    m_enabled = false;
}

void SignalChain::enable(bool v)
{
    m_enabled = v;
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
    if (!m_enabled) {
        return;
    }

    // Prepare audio units update
    foreach (IAudioUnit *pAudioUnit, m_audioUnits) {
        pAudioUnit->prepareUpdate();
    }
}

QList<ISignalChain *> SignalChain::clone(int instances)
{
    // Clone signal chain by serializing and then
    // deserializing it back.

    // Helper structure to keep track of connections
    struct Connection {
        QVariant sourceAudioUnitHandle;
        QVariant targetAudioUnitHandle;
        int sourcePortIndex;
        int targetPortIndex;
    };

    QList<Connection> connections;

    //
    // Serialize
    //

    SerializationContext serContext;
    serContext.serialize(this);

    // Serialize connections
    foreach (IAudioUnit *pIAu, m_audioUnits) {
        AudioUnit *pAu = dynamic_cast<AudioUnit*>(pIAu);
        foreach (InputPort *pInput, pAu->inputs()) {
            if (pInput->connectedOutputPort() != nullptr) {
                IAudioUnit *pSourceAu = pInput->connectedOutputPort()->audioUnit();
                IAudioUnit *pTargetAu = pInput->audioUnit();

                Connection conn;
                conn.sourceAudioUnitHandle = serContext.serialize(dynamic_cast<ISerializable*>(pSourceAu));
                conn.targetAudioUnitHandle = serContext.serialize(dynamic_cast<ISerializable*>(pTargetAu));
                conn.sourcePortIndex = pInput->connectedOutputPort()->index();
                conn.targetPortIndex = pInput->index();
                connections.append(conn);
            }
        }
    }

    //
    // Deserialize
    //

    QList<ISignalChain*> list;

    SignalChainFactory factory;

    // Create instances
    for (int i = 0; i < instances; i++) {

        SerializationContext deserContext(&factory);
        deserContext.fromByteArray(serContext.toByteArray());

        SignalChain *pSignalChainClone = deserContext.deserialize<SignalChain>();

        // Deserialize connections
        foreach(const Connection &conn, connections) {
            AudioUnit *pSourceAu = deserContext.deserialize<AudioUnit>(conn.sourceAudioUnitHandle);
            AudioUnit *pTargetAu = deserContext.deserialize<AudioUnit>(conn.targetAudioUnitHandle);
            OutputPort *pSourcePort = pSourceAu->outputs().at(conn.sourcePortIndex);
            InputPort *pTargetPort = pTargetAu->inputs().at(conn.targetPortIndex);
            pTargetPort->connect(pSourcePort);
        }

        list.append(pSignalChainClone);
    }

    return list;
}

void SignalChain::handleEvent(SignalChainEvent *pEvent)
{
    Q_ASSERT(pEvent != nullptr);
    foreach (IAudioUnit *pAudioUnit, m_audioUnits) {
        pAudioUnit->handleEvent(pEvent);
    }
}

#ifdef PROFILING
void SignalChain::profilingLog()
{
    QList<AudioUnitPlugin*> plugins;
    foreach (IAudioUnit *pAudioUnit, m_audioUnits) {
        AudioUnit *pAu = dynamic_cast<AudioUnit*>(pAudioUnit);
        if (pAu != nullptr) {
            AudioUnitPlugin *pPlugin = pAu->plugin();
            if (!plugins.contains(pPlugin)) {
                plugins.append(pPlugin);
            }
        }
    }

    logInfo("========= profiling =========");
    foreach(AudioUnitPlugin *pPlugin, plugins) {
        pPlugin->profilingLog();
    }

}
#endif // PROFILING

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
