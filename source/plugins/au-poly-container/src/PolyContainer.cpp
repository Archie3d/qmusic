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

    switch (pEvent->type()) {
    case SignalChainEvent::NoteOn: {
        NoteOnEvent *pNoteOnEvent = dynamic_cast<NoteOnEvent*>(pEvent);
        Q_ASSERT(pNoteOnEvent != nullptr);

        int note = pNoteOnEvent->noteNumber();
        if (m_busyVoices.contains(note)) {
            m_busyVoices.value(note)->sendEvent(pEvent->clone());
        } else {
            // New note
            ISignalChain *pVoice = pickFreeVoice();
            if (pVoice != nullptr) {
                pVoice->sendEvent(pEvent->clone());
                m_busyVoices.insert(note, pVoice);
            }
        }
        break;
    }
    case SignalChainEvent::NoteOff: {
        NoteOffEvent *pNoteOffEvent = dynamic_cast<NoteOffEvent*>(pEvent);
        Q_ASSERT(pNoteOffEvent != nullptr);

        int note = pNoteOffEvent->noteNumber();
        ISignalChain *pVoice = m_busyVoices.value(note, nullptr);
        if (pVoice != nullptr) {
            pVoice->sendEvent(pEvent->clone());
        }
        break;
    }
    default:
        // Send all other events to all voices
        foreach (ISignalChain *pSignalChain, m_voices) {
            pSignalChain->sendEvent(pEvent->clone());
        }
        break;
    }

    // Manager busy/free voices
    manageVoices();
}

void PolyphonicContainer::processStart()
{
    // Re-allocate voices if required
    int n = m_pPropNumberOfVoices->value().toInt();
    if (m_voices.count() != n) {
        releaseVoices();
        allocateVoices();
    }
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
        pOutputPort->setValue(0.0f);
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
}

void PolyphonicContainer::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    AudioUnit::deserialize(data, pContext);
    m_pSignalChainScene = pContext->deserialize<SignalChainScene>(data["signalChainScene"]);
    m_pPropLabel->setValue(data["label"]);
    m_pPropNumberOfVoices->setValue(data["voices"]);

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

    pRoot->addSubProperty(m_pPropLabel);
    pRoot->addSubProperty(pPolyphony);
}

void PolyphonicContainer::createVoices(int n)
{
    Q_ASSERT(m_pSignalChainScene != nullptr);
    Q_ASSERT(m_voices.isEmpty());
    Q_ASSERT(n > 0);

    m_voices = m_pSignalChainScene->signalChain()->clone(n);

    foreach (ISignalChain *pVoice, m_voices) {

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

void PolyphonicContainer::updateView()
{
    if (m_pLabelItem != nullptr) {
        // TODO: this is ugly
        m_pLabelItem->setText(QString(" %1").arg(m_pPropLabel->valueText()));
    }
}
