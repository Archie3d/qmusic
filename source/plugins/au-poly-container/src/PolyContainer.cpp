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
#include "SignalChain.h"
#include "SignalChainScene.h"
#include "PolyContainer.h"

const int cNumberOfVoices(8);
const QColor cItemColor(200, 180, 120);

PolyphonicContainer::PolyphonicContainer(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin),
      m_pSignalChainScene(nullptr),
      m_voices()
{
    createProperties();
}

PolyphonicContainer::~PolyphonicContainer()
{
    qDeleteAll(m_voices);
    delete m_pSignalChainScene;
}

void PolyphonicContainer::loadSignalChain(const QString &path)
{
    Q_ASSERT(m_pSignalChainScene == nullptr);
    m_pSignalChainScene = SignalChainScene::loadFromFile(path);
    if (m_pSignalChainScene == nullptr) {
        qCritical() << "Unable to load signal chain from" << path;
        return;
    }

    createVoices();
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
}

void PolyphonicContainer::process()
{
    // TODO: mix the outputs from the signal chains
}

void PolyphonicContainer::reset()
{
    foreach (ISignalChain *pSignalChain, m_voices) {
        pSignalChain->reset();
    }
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
    }
}

