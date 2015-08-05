#include "Application.h"
#include "AudioUnitsManager.h"
#include "AudioUnitPlugin.h"
#include "AudioUnit.h"
#include "SignalChain.h"
#include "SignalChainScene.h"
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
    m_creators[SignalChainAudioUnitItem::UID] = SignalChainAudioUnitItem::create;
}
