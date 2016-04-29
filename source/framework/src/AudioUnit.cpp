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

#ifdef PROFILING
#   include <chrono>
#endif
#include <QtVariantPropertyManager>
#include <QtVariantProperty>
#include "Application.h"
#include "SerializationContext.h"
#include "SignalChain.h"
#include "SignalChainEvent.h"
#include "NoteOnEvent.h"
#include "NoteOffEvent.h"
#include "PitchBendEvent.h"
#include "ControllerEvent.h"
#include "AudioUnitPlugin.h"
#include "AudioUnit.h"

/// Default color used for audio units representation
const QColor cDefaultColor(220, 220, 220);

AudioUnit::AudioUnit(AudioUnitPlugin *pPlugin)
    : m_pSignalChain(nullptr),
      m_pPlugin(pPlugin),
      m_inputs(),
      m_outputs(),
      m_started(false)
{
    Q_ASSERT(pPlugin != nullptr);

    m_pPropertyManager = new QtVariantPropertyManager();
    m_pRootProperty = m_pPropertyManager->addProperty(QtVariantPropertyManager::groupTypeId());
}

AudioUnit::~AudioUnit()
{
    if (m_pSignalChain != nullptr) {
        // Detach audio unit
        m_pSignalChain->removeAudioUnit(this);
    }

    // Delete ports
    qDeleteAll(m_inputs);
    qDeleteAll(m_outputs);

    delete m_pPropertyManager;
}

void AudioUnit::prepareUpdate()
{
    m_updated = false;
}

void AudioUnit::update()
{
    if (m_updated) {
        // Already updated.
        return;
    }

    if (!m_pSignalChain->isEnabled()) {
        // Signal chain is disabled
        return;
    }

    // Activate already the updated flag to prevent loop recursion
    m_updated = true;

    for (InputPort *pInput : m_inputs) {
        pInput->update();
    }
#ifdef PROFILING
    auto startTime = std::chrono::high_resolution_clock::now();
#endif // PROFILING
    process();
#ifdef PROFILING
    auto processTime = std::chrono::high_resolution_clock::now() - startTime;
    double processTimeUs = double(std::chrono::duration_cast<std::chrono::microseconds>(processTime).count());
    profilingRegister(processTimeUs);
#endif // PROFILING
}

void AudioUnit::start()
{
    if (isStarted()) {
        return;
    }

#ifdef PROFILING
    profilingReset();
#endif // PROFILING

    processStart();
    m_started = true;
}

void AudioUnit::stop()
{
    if (!isStarted()) {
        return;
    }

    processStop();
    m_started = false;
}

void AudioUnit::handleEvent(SignalChainEvent *pEvent)
{
    Q_ASSERT(pEvent != nullptr);
    switch (pEvent->type()) {
    case SignalChainEvent::NoteOn:
        noteOnEvent(dynamic_cast<NoteOnEvent*>(pEvent));
        break;
    case SignalChainEvent::NoteOff:
        noteOffEvent(dynamic_cast<NoteOffEvent*>(pEvent));
        break;
    case SignalChainEvent::PitchBend:
        pitchBendEvent(dynamic_cast<PitchBendEvent*>(pEvent));
        break;
    case SignalChainEvent::Controller:
        controllerEvent(dynamic_cast<ControllerEvent*>(pEvent));
        break;
    default:
        // Unknown event.
        break;
    }
}

QColor AudioUnit::color() const
{
    return cDefaultColor;
}

QString AudioUnit::title() const
{
    return m_pPlugin->name();
}

QList<AudioUnit*> AudioUnit::updateChain(const QList<AudioUnit *> chain)
{
    QList<AudioUnit*> newChain(chain);

    if (newChain.contains(this)) {
        return newChain;
    } else {
        newChain.prepend(this);
    }

    // Scan all the inputs of this unit
    for (InputPort *pInputPort : m_inputs) {
        OutputPort *pOutput = pInputPort->connectedOutputPort();
        if (pOutput != nullptr) {
            AudioUnit *pConnectedAu = dynamic_cast<AudioUnit*>(pOutput->audioUnit());
            if (pConnectedAu != nullptr) {
                // Update chain recursively
                newChain = pConnectedAu->updateChain(newChain);
            }
        }
    }

    return newChain;
}

void AudioUnit::fastUpdate()
{
    if (m_updated) {
        // Already updated.
        return;
    }

    if (!m_pSignalChain->isEnabled()) {
        // Signal chain is disabled
        return;
    }

    // Activate already the updated flag to prevent loop recursion
    m_updated = true;

#ifdef PROFILING
    auto startTime = std::chrono::high_resolution_clock::now();
#endif // PROFILING
    process();
#ifdef PROFILING
    auto processTime = std::chrono::high_resolution_clock::now() - startTime;
    double processTimeUs = double(std::chrono::duration_cast<std::chrono::microseconds>(processTime).count());
    profilingRegister(processTimeUs);
#endif // PROFILING
}

InputPort *AudioUnit::addInput(const QString &name, float defaultValue)
{
    InputPort *pInput = new InputPort(name, defaultValue);
    addInput(pInput);
    return pInput;
}

void AudioUnit::addInput(InputPort *pInput)
{
    Q_ASSERT(pInput != nullptr);
    pInput->setAudioUnit(this);
    m_inputs.append(pInput);
}

OutputPort *AudioUnit::addOutput(const QString &name)
{
    OutputPort *pOutput = new OutputPort(name);
    addOutput(pOutput);
    return pOutput;
}

void AudioUnit::addOutput(OutputPort *pOutput)
{
    Q_ASSERT(pOutput != nullptr);
    pOutput->setAudioUnit(this);
    m_outputs.append(pOutput);
}

void AudioUnit::removeAllInputs()
{
    qDeleteAll(m_inputs);
    m_inputs.clear();
}

void AudioUnit::removeAllOutputs()
{
    qDeleteAll(m_outputs);
    m_outputs.clear();
}

void AudioUnit::removeAllPorts()
{
    removeAllInputs();
    removeAllOutputs();
}

QString AudioUnit::uid() const
{
    return m_pPlugin->uid();
}

void AudioUnit::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
}

void AudioUnit::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
}

void AudioUnit::noteOnEvent(NoteOnEvent *pEvent)
{
    Q_ASSERT(pEvent != nullptr);
}

void AudioUnit::noteOffEvent(NoteOffEvent *pEvent)
{
    Q_ASSERT(pEvent != nullptr);
}

void AudioUnit::pitchBendEvent(PitchBendEvent *pEvent)
{
    Q_ASSERT(pEvent != nullptr);
}

void AudioUnit::controllerEvent(ControllerEvent *pEvent)
{
    Q_ASSERT(pEvent != nullptr);
}

#ifdef PROFILING
void AudioUnit::profilingReset()
{
    m_pPlugin->profilingReset();
}

void AudioUnit::profilingRegister(double processTimeUs)
{
    m_pPlugin->profilingRegister(processTimeUs);
}
#endif // PROFILING
