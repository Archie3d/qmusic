#include "SignalChainScene.h"
#include "SignalChainAudioUnitItem.h"
#include "SignalChainConnectionItem.h"
#include "SignalChainPortItem.h"
#include "SignalChain.h"
#include "OutputPort.h"
#include "InputPort.h"
#include "AudioUnit.h"
#include "SerializationContext.h"
#include "SignalChainSceneSelection.h"

const QString SignalChainSceneSelection::UID("signalChainSceneSelection");

SignalChainSceneSelection::SignalChainSceneSelection()
    : m_audioUnitItems(),
      m_connections()
{
}

void SignalChainSceneSelection::populateFromSignalChainScene(const SignalChainScene *pScene, bool selectedOnly)
{
    Q_ASSERT(pScene != nullptr);

    clear();
    QList<QGraphicsItem*> items = selectedOnly ? pScene->selectedItems() : pScene->items();
    QList<ConnectionStruct> connections;
    foreach (QGraphicsItem *pItem, items) {
        switch (pItem->type()) {
        case SignalChainItem::Type_AudioUnit:
            m_audioUnitItems.append(dynamic_cast<SignalChainAudioUnitItem*>(pItem));
            break;
        case SignalChainItem::Type_Connection: {
            SignalChainConnectionItem *pConnectionItem = dynamic_cast<SignalChainConnectionItem*>(pItem);
            ConnectionStruct connStruct;
            connStruct.pSourceAudioUnitItem = pConnectionItem->outputPortItem()->audioUnitItem();
            connStruct.pTargetAudioUnitItem = pConnectionItem->inputPortItem()->audioUnitItem();
            connStruct.sourcePortIndex = pConnectionItem->outputPortItem()->outputPort()->index();
            connStruct.targetPortIndex = pConnectionItem->inputPortItem()->inputPort()->index();
            connections.append(connStruct);
            break;
        }
        default:
            // Unknown items - ignore
            break;
        }
    }

    // Select only those connections that have audio units selected at both ends.
    foreach(const ConnectionStruct &conn, connections) {
        if (m_audioUnitItems.contains(conn.pSourceAudioUnitItem)
                && m_audioUnitItems.contains(conn.pTargetAudioUnitItem)) {
            m_connections.append(conn);
        }
    }
}

void SignalChainSceneSelection::putOnScene(SignalChainScene *pScene, const QPointF &pos)
{
    Q_ASSERT(pScene != nullptr);

    // Unselect items on scene.
    pScene->selectAll(false);

    // Items to be removed from the scene after
    QList<SignalChainAudioUnitItem*> itemsToDelete;

    // Put audio unit items
    foreach (SignalChainAudioUnitItem *pAuItem, m_audioUnitItems) {
        bool itemWillBeDeleted = false;
        if (pAuItem->audioUnit()->flags() && IAudioUnit::Flag_SingleInstance) {
            QString uid = pAuItem->audioUnit()->uid();
            SignalChainAudioUnitItem *pExistingAuItem = pScene->findAudioUnitInstance(uid);
            if (pExistingAuItem != nullptr) {
                itemsToDelete.append(pAuItem);
                itemWillBeDeleted = true;
            }
        }

        if (!itemWillBeDeleted) {
            pScene->signalChain()->addAudioUnit(pAuItem->audioUnit());
            pAuItem->moveBy(pos.x(), pos.y());
            pScene->addItem(pAuItem);
            pAuItem->setSelected(true); // Select the unit put on scene
        }
    }

    // Establish connections
    foreach (const ConnectionStruct &conn, m_connections) {
        SignalChainOutputPortItem *pOutputPortItem = conn.pSourceAudioUnitItem->outputPortItems().at(conn.sourcePortIndex);
        SignalChainInputPortItem *pInputPortItem = conn.pTargetAudioUnitItem->inputPortItems().at(conn.targetPortIndex);
        pScene->connectPorts(pOutputPortItem, pInputPortItem);
    }

    // Remove items to be deleted
    qDeleteAll(itemsToDelete);

    // Refresh the scene.
    pScene->update();
}

bool SignalChainSceneSelection::isEmpty() const
{
    return m_audioUnitItems.isEmpty() && m_connections.isEmpty();
}

void SignalChainSceneSelection::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);

    QPointF groupPosition;
    int groupSize = 0;

    QVariantList auList;
    foreach (SignalChainAudioUnitItem *pAuItem, m_audioUnitItems) {
        groupPosition += pAuItem->pos();
        groupSize++;
        auList.append(pContext->serialize(pAuItem));
    }

    // Audio unit items center position.
    if (groupSize > 0) {
        groupPosition = QPointF(groupPosition.x() / groupSize,
                                groupPosition.y() / groupSize);
    }

    QVariantList connectionList;
    foreach (const ConnectionStruct &conn, m_connections) {
        QVariantMap map;
        map["sourceAudioUnitItem"] = pContext->serialize(conn.pSourceAudioUnitItem);
        map["targetAudioUnitItem"] = pContext->serialize(conn.pTargetAudioUnitItem);
        map["sourcePortIndex"] = conn.sourcePortIndex;
        map["targetPortIndex"] = conn.targetPortIndex;
        connectionList.append(map);
    }

    data["audioUnitItems"] = auList;
    data["connections"] = connectionList;
    data["groupPosition"] = groupPosition;
}

void SignalChainSceneSelection::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);

    clear();

    QPointF groupPosition = data["groupPosition"].value<QPointF>();

    // Deserialize audio unit items
    QVariantList auList = data["audioUnitItems"].toList();
    foreach (const QVariant &handle, auList) {
        SignalChainAudioUnitItem *pAuItem = pContext->deserialize<SignalChainAudioUnitItem>(handle);
        Q_ASSERT(pAuItem != nullptr);
        pAuItem->setPos(pAuItem->pos() - groupPosition);    // Adjust position to the group center
        m_audioUnitItems.append(pAuItem);
    }

    // Deserialize connections
    QVariantList connectionList = data["connections"].toList();
    foreach (const QVariant &v, connectionList) {
        QVariantMap map = v.toMap();
        ConnectionStruct conn;
        conn.pSourceAudioUnitItem = pContext->deserialize<SignalChainAudioUnitItem>(map["sourceAudioUnitItem"]);
        conn.pTargetAudioUnitItem = pContext->deserialize<SignalChainAudioUnitItem>(map["targetAudioUnitItem"]);
        Q_ASSERT(conn.pSourceAudioUnitItem != nullptr);
        Q_ASSERT(conn.pTargetAudioUnitItem != nullptr);
        conn.sourcePortIndex = map["sourcePortIndex"].toInt();
        conn.targetPortIndex = map["targetPortIndex"].toInt();
        m_connections.append(conn);
    }
}

void SignalChainSceneSelection::clear()
{
    m_audioUnitItems.clear();
    m_connections.clear();
}
