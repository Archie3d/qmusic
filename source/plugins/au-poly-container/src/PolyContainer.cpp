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
#include "Application.h"
#include "SerializationContext.h"
#include "ExposedInput.h"
#include "ExposedOutput.h"
#include "SignalChain.h"
#include "SignalChainScene.h"
#include "SignalChainEvent.h"
#include "PolyContainer.h"

const int cNumberOfVoices(8);
const QColor cItemColor(220, 200, 150);
const QString cExposeInputUid("b12c76c4ee191b4452ed951a270b4645");
const QString cExposeOutputUid("0a3872cffcd4f8d00843016dc031c5d4");

PolyphonicContainer::PolyphonicContainer(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin),
      m_pSignalChainScene(nullptr),
      m_voices(),
      m_freeVoices(),
      m_busyVoices()
{
    createProperties();
}

PolyphonicContainer::~PolyphonicContainer()
{
    qDeleteAll(m_voices);
    delete m_pSignalChainScene;
}

void PolyphonicContainer::setSignalChainScene(SignalChainScene *pScene)
{
    Q_ASSERT(pScene != nullptr);
    Q_ASSERT(m_pSignalChainScene == nullptr);

    m_pSignalChainScene = pScene;

    createPorts();
    createVoices();
}

void PolyphonicContainer::handleEvent(SignalChainEvent *pEvent)
{
    Q_ASSERT(pEvent);

    QString name = pEvent->name();
    if (name == "noteOn") {
        int note = pEvent->data().toMap()["number"].toInt();
        if (m_busyVoices.contains(note)) {
            m_busyVoices.value(note)->sendEvent(new SignalChainEvent(*pEvent));
        } else {
            // New note
            ISignalChain *pVoice = pickFreeVoice();
            if (pVoice != nullptr) {
                pVoice->sendEvent(new SignalChainEvent(*pEvent));
                m_busyVoices.insert(note, pVoice);
            }
        }
    } else if (name == "noteOff") {
        int note = pEvent->data().toMap()["number"].toInt();
        ISignalChain *pVoice = m_busyVoices.value(note, nullptr);
        if (pVoice != nullptr) {
            pVoice->sendEvent(new SignalChainEvent(*pEvent));
        }
    } else {
        // Send all other events to all voices
        foreach (ISignalChain *pSignalChain, m_voices) {
            pSignalChain->sendEvent(new SignalChainEvent(*pEvent));
        }
    }

    manageVoices();
}

void PolyphonicContainer::processStart()
{
    foreach (ISignalChain *pSignalChain, m_voices) {
        pSignalChain->setTimeStep(signalChain()->timeStep());
        pSignalChain->start();
    }
}

void PolyphonicContainer::processStop()
{
    foreach (ISignalChain *pSignalChain, m_voices) {
        pSignalChain->stop();
    }
    freeAllVoices();
}

void PolyphonicContainer::process()
{
    prepareVoicesUpdate();

    foreach (OutputPort *pOutputPort, m_outputs) {
        pOutputPort->setFloatValue(0.0f);
    }

    // The following will trigger update of internal signal chains
    foreach (IAudioUnit *pAu, m_exposeOutputAudioUnits) {
        pAu->update();
    }
}

void PolyphonicContainer::reset()
{
    foreach (ISignalChain *pSignalChain, m_voices) {
        pSignalChain->reset();
    }

    freeAllVoices();
}

QColor PolyphonicContainer::color() const
{
    return cItemColor;
}

int PolyphonicContainer::flags() const
{
    return Flag_NoFlags;
}

void PolyphonicContainer::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    AudioUnit::serialize(data, pContext);
    data["signalChainScene"] = pContext->serialize(m_pSignalChainScene);
}

void PolyphonicContainer::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    AudioUnit::deserialize(data, pContext);
    m_pSignalChainScene = pContext->deserialize<SignalChainScene>(data["signalChainScene"]);

    createPorts();
    createVoices();
}

void PolyphonicContainer::createProperties()
{
    //QtVariantProperty *pRoot = rootProperty();
}

void PolyphonicContainer::createVoices()
{
    Q_ASSERT(m_pSignalChainScene != nullptr);
    for (int i = 0; i < cNumberOfVoices; i++) {
        ISignalChain *pVoice = m_pSignalChainScene->signalChain()->clone();
        Q_ASSERT(pVoice != nullptr);
        m_voices.append(pVoice);

        // Add to the list og free voices
        m_freeVoices.append(pVoice);

        int inputIndex = 0;
        int outputIndex = 0;

        // Reference voice ports
        QList<IAudioUnit*> audioUnits = pVoice->audioUnits();
        foreach (IAudioUnit *pAu, audioUnits) {
            if (pAu->uid() == cExposeInputUid) {
                ExposedInput *pExpInput = dynamic_cast<ExposedInput*>(pAu);
                Q_ASSERT(pExpInput != nullptr);
                pExpInput->setRefInputPort(m_inputs.at(inputIndex++));

            } else if (pAu->uid() == cExposeOutputUid) {
                ExposedOutput *pExpOutput = dynamic_cast<ExposedOutput*>(pAu);
                Q_ASSERT(pExpOutput != nullptr);
                pExpOutput->setRefOutputPort(m_outputs.at(outputIndex++));
                m_exposeOutputAudioUnits.append(pAu);
            }
        }
    }
}

void PolyphonicContainer::createPorts()
{
    Q_ASSERT(m_pSignalChainScene != nullptr);

    QList<IAudioUnit*> audioUnits = m_pSignalChainScene->signalChain()->audioUnits();
    foreach (IAudioUnit *pAu, audioUnits) {
        if (pAu->uid() == cExposeInputUid) {
            ExposedInput *pExpInput = dynamic_cast<ExposedInput*>(pAu);
            Q_ASSERT(pExpInput != nullptr);
            InputPort *pInputPort = addInput(pExpInput->exposedInputName(), Signal::Type_Float);
            m_inputs.append(pInputPort);
        } else if (pAu->uid() == cExposeOutputUid) {
            ExposedOutput *pExpOutput = dynamic_cast<ExposedOutput*>(pAu);
            Q_ASSERT(pExpOutput != nullptr);
            OutputPort *pOutputPort = addOutput(pExpOutput->exposedOutputName(), Signal::Type_Float);
            m_outputs.append(pOutputPort);
        }
    }
}

void PolyphonicContainer::prepareVoicesUpdate()
{
    foreach (ISignalChain *pSignalChain, m_voices) {
        pSignalChain->prepareUpdate();
    }
}

void PolyphonicContainer::manageVoices()
{
    // Move all already disabled voices from busy to free list
    QMap<int, ISignalChain*>::iterator it = m_busyVoices.begin();
    while (it != m_busyVoices.end()) {
        ISignalChain *pVoice = it.value();
        if (!pVoice->isEnabled()) {
            // Voice is disabled
            it = m_busyVoices.erase(it);
            m_freeVoices.append(pVoice);
        } else {
            ++it;
        }
    }
}

void PolyphonicContainer::freeAllVoices()
{
    m_busyVoices.clear();
    m_freeVoices = m_voices;
}

ISignalChain* PolyphonicContainer::pickFreeVoice()
{
    if (m_freeVoices.isEmpty()) {
        return nullptr;
    }
    ISignalChain *pVoice = m_freeVoices.first();
    m_freeVoices.removeOne(pVoice);

    return pVoice;
}

