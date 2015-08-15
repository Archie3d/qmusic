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

#include "Application.h"
#include "AudioUnitsManager.h"
#include "AudioUnitPlugin.h"
#include "AudioUnit.h"
#include "SignalChain.h"
#include "SignalChainScene.h"
#include "SignalChainSceneSelection.h"
#include "SignalChainAudioUnitItem.h"
#include "SignalChainFactory.h"

SignalChainFactory::SignalChainFactory()
{
    registerCreators();
}

ISerializable* SignalChainFactory::createObject(const QString &uid) const
{
    if (m_creators.contains(uid)) {
        return m_creators[uid]();
    }

    // Create audio unit by its plugin UID
    AudioUnitPlugin *pPlugin = Application::instance()->audioUnitsManager()->audioUnitPluginByUid(uid);
    if (pPlugin != nullptr) {
        return pPlugin->createInstance();
    }

    return nullptr;
}

void SignalChainFactory::registerCreators()
{
    m_creators[SignalChain::UID] = SignalChain::create;
    m_creators[SignalChainScene::UID] = SignalChainScene::create;
    m_creators[SignalChainSceneSelection::UID] = SignalChainSceneSelection::create;
    m_creators[SignalChainAudioUnitItem::UID] = SignalChainAudioUnitItem::create;
}
