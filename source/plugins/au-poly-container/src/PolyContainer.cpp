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
#include "NoteOnEvent.h"
#include "NoteOffEvent.h"
#include "PolyContainer.h"

const int cNumberOfVoices(8);
const QColor cItemColor(220, 200, 160);
const QString cExposeInputUid("b12c76c4ee191b4452ed951a270b4645");
const QString cExposeOutputUid("0a3872cffcd4f8d00843016dc031c5d4");

PolyphonicContainer::PolyphonicContainer(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin),
      m_pSignalChainScene(nullptr),
      m_voices(),
      m_freeVoices(),
      m_busyVoices(),
      m_pLabelItem(nullptr)
{    
    createProperties();
}

PolyphonicContainer::~PolyphonicContainer()
{
    releaseVoices();
    delete m_pSignalChainScene;
}

void PolyphonicContainer::setSignalChainScene(SignalChainScene *pScene)
{
    Q_ASSERT(pScene != nullptr);
    Q_ASSERT(m_pSignalChainScene == nullptr);

    m_pSignalChainScene = pScene;

    createPorts();
}

void PolyphonicContainer::handleEvent(SignalChainEvent *pEvent)
{
    Q_ASSERT(pEvent);

    QMutexLocker locker(&m_containerMutex);

    switch (pEvent->type()) {
    case SignalChainEvent::NoteOn: {
        NoteOnEvent *pNoteOnEvent = dynamic_cast<NoteOnEvent*>(pEvent);
        Q_ASSERT(pNoteOnEvent != nullptr);

        int note = pNoteOnEvent->noteNumber();
        ISignalChain *pVoice = findBusyVoice(note);
        if (pVoice != nullptr) {
            pVoice->handleEvent(pEvent);
        } else {
            // New note
            ISignalChain *pVoice = pickFreeVoice();
            if (pVoice != nullptr) {
                pVoice->handleEvent(pEvent);
                m_busyVoices.append(TheVoice(note, pVoice));
            }
        }
        break;
    }
    case SignalChainEvent::NoteOff: {
        NoteOffEvent *pNoteOffEvent = dynamic_cast<NoteOffEvent*>(pEvent);
        Q_ASSERT(pNoteOffEvent != nullptr);

        int note = pNoteOffEvent->noteNumber();
        ISignalChain *pVoice = findBusyVoice(note);
        if (pVoice != nullptr) {
            pVoice->handleEvent(pEvent);
        }
        break;
    }
    default:
        // Send all other events to all voices
        for (ISignalChain *pSignalChain : m_voices) {
            pSignalChain->handleEvent(pEvent);
        }
        break;
    }

    // Manager busy/free voices
    manageVoices();
}

void PolyphonicContainer::setLabel(const QString &text)
{
    m_pPropLabel->setValue(text);
}

void PolyphonicContainer::processStart()
{
    // Re-allocate voices if required
    int n = m_pPropNumberOfVoices->value().toInt();
    if (m_voices.count() != n) {
        releaseVoices();
        allocateVoices();
    }
    m_voiceStealing = m_pPropStealVoice->value().toBool();

    for (ISignalChain *pSignalChain : m_voices) {
        pSignalChain->setTimeStep(signalChain()->timeStep());
        pSignalChain->start();
    }
}

void PolyphonicContainer::processStop()
{
    for (ISignalChain *pSignalChain : m_voices) {
        pSignalChain->stop();
    }
    freeAllVoices();
}

void PolyphonicContainer::process()
{
    QMutexLocker locker(&m_containerMutex);

    prepareVoicesUpdate();

    // Set outputs to zero, since
    // further all voices will add up to them
    for (OutputPort *pOutputPort : m_outputs) {
        pOutputPort->setValue(0.0f);
    }

    // The following will trigger update of internal signal chains.
    for (ExposedOutput *pAu : m_exposeOutputAudioUnits) {
        // Optimization: we only update the enabled (sounding) signal chains.
        if (pAu->signalChain()->isEnabled()) {
            pAu->fastUpdate();
        }
    }
}

void PolyphonicContainer::reset()
{
    for (ISignalChain *pSignalChain : m_voices) {
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

QGraphicsItem* PolyphonicContainer::graphicsItem()
{
    if (m_pLabelItem == nullptr) {
        m_pLabelItem = new QGraphicsSimpleTextItem();
        m_pLabelItem->setBrush(QBrush(QColor(80, 40, 80)));
        updateView();
    }
    return m_pLabelItem;
}

void PolyphonicContainer::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    AudioUnit::serialize(data, pContext);
    data["signalChainScene"] = pContext->serialize(m_pSignalChainScene);
    data["label"] = m_pPropLabel->value();
    data["voices"] = m_pPropNumberOfVoices->value();
    data["voiceStealing"] = m_pPropStealVoice->value();
}

void PolyphonicContainer::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    AudioUnit::deserialize(data, pContext);
    m_pSignalChainScene = pContext->deserialize<SignalChainScene>(data["signalChainScene"]);
    m_pPropLabel->setValue(data["label"]);
    m_pPropNumberOfVoices->setValue(data["voices"]);
    m_pPropStealVoice->setValue(data["voiceStealing"]);

    createPorts();
}

void PolyphonicContainer::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();

    m_pPropLabel = propertyManager()->addProperty(QVariant::String, "Label");
    QObject::connect(propertyManager(), &QtVariantPropertyManager::propertyChanged, [this](QtProperty *pProperty) {
        QtVariantProperty *pV = dynamic_cast<QtVariantProperty*>(pProperty);
        if (pV == m_pPropLabel) {
            updateView();
        }
    });

    QtVariantProperty *pPolyphony = propertyManager()->addProperty(propertyManager()->groupTypeId(), "Polyphony");

    m_pPropNumberOfVoices = propertyManager()->addProperty(QVariant::Int, "Voices");
    m_pPropNumberOfVoices->setAttribute("minimum", 1);
    m_pPropNumberOfVoices->setAttribute("maximum", 24);
    m_pPropNumberOfVoices->setValue(8);
    pPolyphony->addSubProperty(m_pPropNumberOfVoices);

    m_pPropStealVoice = propertyManager()->addProperty(QVariant::Bool, "Voice stealing");
    m_pPropStealVoice->setValue(false);
    pPolyphony->addSubProperty(m_pPropStealVoice);

    pRoot->addSubProperty(m_pPropLabel);
    pRoot->addSubProperty(pPolyphony);
}

void PolyphonicContainer::createVoices(int n)
{
    Q_ASSERT(m_pSignalChainScene != nullptr);
    Q_ASSERT(m_voices.isEmpty());
    Q_ASSERT(n > 0);

    m_voices = m_pSignalChainScene->signalChain()->clone(n);

    for (ISignalChain *pVoice : m_voices) {

        // Add to the list of free voices
        m_freeVoices.append(pVoice);

        int inputIndex = 0;
        int outputIndex = 0;

        // Reference voice ports
        QList<IAudioUnit*> audioUnits = pVoice->audioUnits();
        for (IAudioUnit *pAu : audioUnits) {
            if (pAu->uid() == cExposeInputUid) {
                ExposedInput *pExpInput = dynamic_cast<ExposedInput*>(pAu);
                Q_ASSERT(pExpInput != nullptr);
                pExpInput->setRefInputPort(m_inputs.at(inputIndex++));

            } else if (pAu->uid() == cExposeOutputUid) {
                ExposedOutput *pExpOutput = dynamic_cast<ExposedOutput*>(pAu);
                Q_ASSERT(pExpOutput != nullptr);
                pExpOutput->setRefOutputPort(m_outputs.at(outputIndex++));
                m_exposeOutputAudioUnits.append(pExpOutput);
            }
        }
    }
}

void PolyphonicContainer::createPorts()
{
    Q_ASSERT(m_pSignalChainScene != nullptr);

    QList<IAudioUnit*> audioUnits = m_pSignalChainScene->signalChain()->audioUnits();
    for (IAudioUnit *pAu : audioUnits) {
        if (pAu->uid() == cExposeInputUid) {
            ExposedInput *pExpInput = dynamic_cast<ExposedInput*>(pAu);
            Q_ASSERT(pExpInput != nullptr);
            InputPort *pInputPort = addInput(pExpInput->exposedInputName());
            m_inputs.append(pInputPort);
        } else if (pAu->uid() == cExposeOutputUid) {
            ExposedOutput *pExpOutput = dynamic_cast<ExposedOutput*>(pAu);
            Q_ASSERT(pExpOutput != nullptr);
            OutputPort *pOutputPort = addOutput(pExpOutput->exposedOutputName());
            m_outputs.append(pOutputPort);
        }
    }
}

void PolyphonicContainer::prepareVoicesUpdate()
{
    // Prepare updating the busy (playing) voices only.
    for (auto voice : m_busyVoices) {
        voice.second->prepareUpdate();
   }
}

void PolyphonicContainer::manageVoices()
{
    // Move all already disabled voices from busy to free list
    QList<TheVoice>::iterator it = m_busyVoices.begin();
    while (it != m_busyVoices.end()) {
        ISignalChain *pVoice = (*it).second;
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

void PolyphonicContainer::allocateVoices()
{
    Q_ASSERT(m_voices.isEmpty());

    int n = m_pPropNumberOfVoices->value().toInt();

    createVoices(n);
}

void PolyphonicContainer::releaseVoices()
{
    qDeleteAll(m_voices);
    m_voices.clear();
    m_busyVoices.clear();
    m_freeVoices.clear();
    m_exposeOutputAudioUnits.clear();
}

ISignalChain* PolyphonicContainer::findBusyVoice(int noteNumber)
{
    for (const TheVoice &voice : m_busyVoices) {
        if (voice.first == noteNumber) {
            return voice.second;
        }
    }
    return nullptr;
}

ISignalChain* PolyphonicContainer::pickFreeVoice()
{
    if (m_freeVoices.isEmpty()) {
        if (m_voiceStealing && !m_busyVoices.isEmpty()) {
            // Streal the oldest voice
            TheVoice voice = m_busyVoices.first();
            m_busyVoices.removeFirst();

            // Make sure the voice note is turned off
            NoteOffEvent noteOffEvent(voice.first, 64);
            voice.second->handleEvent(&noteOffEvent);

            return voice.second;
        }
        return nullptr;
    }

    ISignalChain *pVoice = m_freeVoices.first();
    m_freeVoices.removeOne(pVoice);

    return pVoice;
}

void PolyphonicContainer::updateView()
{
    if (m_pLabelItem != nullptr) {
        // TODO: this is ugly
        m_pLabelItem->setText(QString(" %1").arg(m_pPropLabel->valueText()));
    }
}
