#include "Application.h"
#include "AudioUnitsManager.h"
#include "AudioUnitPlugin.h"
#include "AudioUnit.h"
#include "InputPort.h"
#include "OutputPort.h"
#include "SignalChain.h"
#include "SignalChainScene.h"
#include "SignalChainPortItem.h"
#include "SignalChainConnectionItem.h"
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
    m_creators[SignalChainInputPortItem::UID] = SignalChainInputPortItem::create;
    m_creators[SignalChainOutputPortItem::UID] = SignalChainOutputPortItem::create;
    m_creators[SignalChainConnectionItem::UID] = SignalChainConnectionItem::create;
    m_creators[InputPort::UID] = InputPort::create;
    m_creators[OutputPort::UID] = OutputPort::create;
}
