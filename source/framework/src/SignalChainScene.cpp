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
#include <QPainter>
#include <QMimeData>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QKeyEvent>
#include <QClipboard>
#include "Application.h"
#include "SerializationContext.h"
#include "SerializationFile.h"
#include "SignalChainFactory.h"
#include "AudioUnitsManager.h"
#include "AudioUnitPlugin.h"
#include "AudioUnit.h"
#include "ISignalChainSceneContainer.h"
#include "SignalChain.h"
#include "SignalChainItem.h"
#include "SignalChainPortItem.h"
#include "SignalChainConnectionItem.h"
#include "SignalChainAudioUnitItem.h"
#include "SignalChainSceneSelection.h"
#include "SignalChainScene.h"

const QSizeF cGridSize(8, 8);
const QColor cGridColor(250, 250, 250);

const QString SignalChainScene::UID("SignalChainScene");
const quint32 SignalChainScene_Magic(0x7af98ed8);

const QString cSignalChainMimeDataId("qmusic/signalChainSceneItems");

SignalChainScene::SignalChainScene(QObject *pParent)
    : QGraphicsScene(pParent)
{
    m_pSignalChain = new SignalChain();
    m_pDraggedAudioUnitPlugin = nullptr;
    m_pConnectionItem = nullptr;

    connect(this, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));
}

SignalChainScene::~SignalChainScene()
{
    m_pSignalChain->stop();
    deleteAll();

    delete m_pSignalChain;
}

SignalChainItem* SignalChainScene::signalChainItemAtPos(const QPointF &pos) const
{
    QList<QGraphicsItem*> graphicsItems = items(QRectF(pos - QPointF(1, 1), QSizeF(3, 3)));
    foreach (QGraphicsItem *pItem, graphicsItems) {
        if (pItem->type() >= SignalChainItem::Type_First) {
            return dynamic_cast<SignalChainItem*>(pItem);
        }
    }
    return nullptr;
}

bool SignalChainScene::saveToFile(const QString &path)
{
    SerializationContext context;
    context.serialize(this);

    SerializationFile file(path);
    file.setMagic(SignalChainScene_Magic);
    file.setBuffer(context.toByteArray());

    return file.save();
}

SignalChainScene* SignalChainScene::loadFromFile(const QString &path)
{
    SerializationFile file(path);
    if (!file.load()) {
        qCritical() << "Unable to load" << path;
        return nullptr;
    }

    SignalChainFactory factory;
    SerializationContext context(&factory);
    context.fromByteArray(file.buffer());
    SignalChainScene *pScene = context.deserialize<SignalChainScene>();
    if (pScene == nullptr) {
        qCritical() << "Unable to load signal chain from file" << path;
        return nullptr;
    }

    return pScene;
}

void SignalChainScene::selectAll(bool select)
{
    foreach (QGraphicsItem *pItem, items(Qt::DescendingOrder)) {
        pItem->setSelected(select);
    }
}

void SignalChainScene::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);

    data["signalChain"] = pContext->serialize(m_pSignalChain);
    data["audioUnitItems"] = serializeAudioUnitItems(pContext);
    data["connections"] = serializeConnections(pContext);
}

void SignalChainScene::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);

    // Remove all existing items
    deleteAll();

    // Deserialize audio units
    delete m_pSignalChain;
    m_pSignalChain = pContext->deserialize<SignalChain>(data["signalChain"]);

    // Deserialize audio unit items
    deserializeAudioUnitItems(data["audioUnitItems"], pContext);

    // Deserialize connections
    deserializeConnections(data["connections"], pContext);
}

void SignalChainScene::deleteSelected()
{
    QSet<SignalChainConnectionItem*> connectionsToDelete;
    QSet<SignalChainAudioUnitItem*> audioUnitsToDelete;

    foreach (QGraphicsItem *pItem, selectedItems()) {
        if (pItem->type() == SignalChainItem::Type_AudioUnit) {
            SignalChainAudioUnitItem *pAuItem = dynamic_cast<SignalChainAudioUnitItem*>(pItem);
            audioUnitsToDelete.insert(pAuItem);
            foreach (SignalChainConnectionItem *pConnItem, pAuItem->connectionItems()) {
                connectionsToDelete.insert(pConnItem);
            }
        } else if (pItem->type() == SignalChainItem::Type_Connection) {
            SignalChainConnectionItem *pConnectionItem = dynamic_cast<SignalChainConnectionItem*>(pItem);
            connectionsToDelete.insert(pConnectionItem);
        }
    }

    foreach (SignalChainConnectionItem *pItem, connectionsToDelete) {
        delete pItem;
    }

    foreach (SignalChainAudioUnitItem *pItem, audioUnitsToDelete) {
        delete pItem;
    }
}

void SignalChainScene::deleteAll()
{
    clear();
}

void SignalChainScene::copyToClipboard()
{
    // Serialize selected items to byte array
    QByteArray data = serializeToByteArray(true);
    if (data.isEmpty()) {
        return;
    }

    QMimeData *pMimeData = new QMimeData();
    pMimeData->setData(cSignalChainMimeDataId, data);
    QClipboard *pClipboard = QApplication::clipboard();
    pClipboard->setMimeData(pMimeData);
}

void SignalChainScene::pasteFromClipboard()
{
    QByteArray data = Application::instance()->clipboardData(cSignalChainMimeDataId);
    deserializeFromByteArray(data);
}

void SignalChainScene::mousePressEvent(QGraphicsSceneMouseEvent *pEvent)
{
    if (pEvent->button() == Qt::LeftButton && !m_pSignalChain->isStarted()) {
        SignalChainItem *pItem = signalChainItemAtPos(pEvent->scenePos());
        if (pItem != nullptr) {
            if (pItem->type() == SignalChainItem::Type_InputPort ||
                    pItem->type() == SignalChainItem::Type_OutputPort) {
                SignalChainPortItem *pPortItem = dynamic_cast<SignalChainPortItem*>(pItem);
                Q_ASSERT(pPortItem != nullptr);
                if (pPortItem->isOutput() || (pPortItem->isInput() && !pPortItem->hasConnections())) {
                    m_pConnectionItem = new SignalChainConnectionItem();
                    addItem(m_pConnectionItem);
                    m_pConnectionItem->setPort(pPortItem);
                    m_pConnectionItem->updatePath();
                    emit beginConnection();
                    return;
                }
            }
        }
    }

    QGraphicsScene::mousePressEvent(pEvent);
}

void SignalChainScene::mouseMoveEvent(QGraphicsSceneMouseEvent *pEvent)
{
    m_mousePos = pEvent->scenePos();

    if (m_pConnectionItem != nullptr) {
        if (m_pConnectionItem->inputPortItem() != nullptr) {
            m_pConnectionItem->setSourcePoint(m_mousePos);
        } else if (m_pConnectionItem->outputPortItem() != nullptr) {
            m_pConnectionItem->setTargetPoint(m_mousePos);
        }
        m_pConnectionItem->updatePath();
        return;
    }

    QGraphicsScene::mouseMoveEvent(pEvent);
}

void SignalChainScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent)
{
    if (pEvent->button() == Qt::LeftButton) {
        if (m_pConnectionItem != nullptr) {
            SignalChainItem *pItem = signalChainItemAtPos(pEvent->scenePos());
            if (pItem != nullptr) {
                if (pItem->type() == SignalChainItem::Type_InputPort) {
                    SignalChainInputPortItem *pInputItem = dynamic_cast<SignalChainInputPortItem*>(pItem);
                    if (!pInputItem->hasConnections() &&
                            m_pConnectionItem->inputPortItem() == nullptr &&
                            m_pConnectionItem->outputPortItem()->parentItem() != pInputItem->parentItem()) {
                        finilizeConnection(pInputItem);
                        return;
                    }
                } else if (pItem->type() == SignalChainItem::Type_OutputPort) {
                    SignalChainOutputPortItem *pOutputItem = dynamic_cast<SignalChainOutputPortItem*>(pItem);
                    if (m_pConnectionItem->outputPortItem() == nullptr &&
                            m_pConnectionItem->inputPortItem()->parentItem() != pOutputItem->parentItem()) {
                        finilizeConnection(pOutputItem);
                        return;
                    }
                }
            }

            delete m_pConnectionItem;
            m_pConnectionItem = nullptr;
            emit endConnection();
            return;
        }
    }

    QGraphicsScene::mouseReleaseEvent(pEvent);
}

void SignalChainScene::dragEnterEvent(QGraphicsSceneDragDropEvent *pEvent)
{
    bool acceptDrag = false;

    if (!m_pSignalChain->isStarted()) {
        const QMimeData *pMimeData = pEvent->mimeData();
        if (pMimeData->formats().contains(AudioUnitPlugin::MimeDataFormat)) {
            QString uid = QString::fromUtf8(pMimeData->data(AudioUnitPlugin::MimeDataFormat));
            SignalChainAudioUnitItem *pExistingItem = findAudioUnitInstance(uid);
            if (pExistingItem != nullptr && (pExistingItem->audioUnit()->flags() & IAudioUnit::Flag_SingleInstance) != 0) {
                // Instance already exits
                QString name = Application::instance()->audioUnitsManager()->audioUnitPluginByUid(uid)->name();
                qDebug() << "Only a single instance of" << name << "item is allowed";
            } else {
                m_pDraggedAudioUnitPlugin = Application::instance()->audioUnitsManager()->audioUnitPluginByUid(uid);
            }
            acceptDrag = m_pDraggedAudioUnitPlugin != nullptr;
        }
    }
    pEvent->setAccepted(acceptDrag);
}

void SignalChainScene::dragMoveEvent(QGraphicsSceneDragDropEvent *pEvent)
{
    bool accept = !m_pSignalChain->isStarted() && (m_pDraggedAudioUnitPlugin != nullptr) && (m_pSignalChain != nullptr);
    pEvent->setAccepted(accept);
}

void SignalChainScene::dropEvent(QGraphicsSceneDragDropEvent *pEvent)
{
    AudioUnit *pAudioUnit = nullptr;

    if (m_pDraggedAudioUnitPlugin != nullptr && m_pSignalChain != nullptr) {
        pAudioUnit = m_pDraggedAudioUnitPlugin->createInstanceInteractive();
        m_pDraggedAudioUnitPlugin = nullptr;
    }

    if (pAudioUnit != nullptr) {
        Q_ASSERT(m_pSignalChain != nullptr);
        m_pSignalChain->addAudioUnit(pAudioUnit);
        SignalChainAudioUnitItem *pItem = new SignalChainAudioUnitItem(pAudioUnit);
        QPointF pos = pEvent->scenePos();
        pItem->setPos(pos);
        addItem(pItem);
        pEvent->setAccepted(true);
    } else {
        pEvent->setAccepted(false);
    }
}

void SignalChainScene::keyPressEvent(QKeyEvent *pEvent)
{
    if (!m_pSignalChain->isStarted()) {
        if (pEvent->key() == Qt::Key_Delete) {
            deleteSelected();
        } else if (pEvent->matches(QKeySequence::SelectAll)) {
            selectAll();
        } else if (pEvent->matches(QKeySequence::Copy)) {
            copyToClipboard();
        } else if (pEvent->matches(QKeySequence::Paste)) {
            pasteFromClipboard();
        } else if (pEvent->matches(QKeySequence::Cut)) {
            copyToClipboard();
            deleteSelected();
        }
    }
    QGraphicsScene::keyPressEvent(pEvent);

}

void SignalChainScene::drawBackground(QPainter *pPainter, const QRectF &rect)
{
    pPainter->save();
    pPainter->setRenderHint(QPainter::Antialiasing, false);

    qreal scale = pPainter->transform().m11();

    QPen pen;
    pen.setWidthF(1.0 / scale);
    pen.setColor(cGridColor);
    pPainter->setPen(pen);

    int top = qRound(rect.top() / cGridSize.height()) * cGridSize.height();
    int left = qRound(rect.left() / cGridSize.width()) * cGridSize.width();
    int bottom = qRound((rect.bottom() + cGridSize.height()) / cGridSize.height()) * cGridSize.height();
    int right = qRound((rect.right() + cGridSize.width()) / cGridSize.width()) * cGridSize.width();

    int sx = cGridSize.width();
    int sy = cGridSize.height();

    for (int y = top; y < bottom; y += sy) {
        pPainter->drawLine(left, y, right, y);
    }

    for (int x = left; x < right; x += sx) {
        pPainter->drawLine(x, top, x, bottom);
    }

    pPainter->restore();
}

void SignalChainScene::onSelectionChanged()
{
    QList<QGraphicsItem*> items = selectedItems();

    if (items.count() == 1) {
        QGraphicsItem *pItem = items.at(0);
        if (pItem->type() == SignalChainItem::Type_AudioUnit) {
            SignalChainAudioUnitItem *pAuItem = dynamic_cast<SignalChainAudioUnitItem*>(pItem);
            Q_ASSERT(pAuItem != nullptr);
            emit audioUnitSelected(pAuItem->audioUnit());
            return;
        }
    }

    // No audio unit is selected
    emit audioUnitSelected(nullptr);
}

void SignalChainScene::connectPorts(SignalChainOutputPortItem *pOutputPort, SignalChainInputPortItem *pInputPort)
{
    Q_ASSERT(pOutputPort != nullptr);
    Q_ASSERT(pInputPort != nullptr);

    if (pInputPort->hasConnections()) {
        // Input ports can have only one incoming connection
        return;
    }

    // Create connection path and connect ports
    SignalChainConnectionItem *pConnectionItem = new SignalChainConnectionItem();
    addItem(pConnectionItem);
    pConnectionItem->setSourcePort(pOutputPort);
    pConnectionItem->setTargetPort(pInputPort);
    pInputPort->inputPort()->connect(pOutputPort->outputPort());
    pConnectionItem->updatePathFromPorts();
}

void SignalChainScene::finilizeConnection(SignalChainPortItem *pFinalPort)
{
    Q_ASSERT(m_pConnectionItem != nullptr);
    Q_ASSERT(pFinalPort != nullptr);

    m_pConnectionItem->setPort(pFinalPort);
    m_pConnectionItem->updatePath();

    // Establish connection at signal chain
    InputPort *pInputPort = m_pConnectionItem->inputPortItem()->inputPort();
    OutputPort *pOutputPort = m_pConnectionItem->outputPortItem()->outputPort();
    Q_ASSERT(pInputPort != nullptr);
    Q_ASSERT(pOutputPort != nullptr);
    pInputPort->connect(pOutputPort);

    m_pConnectionItem = nullptr;
    emit endConnection();
}

QVariant SignalChainScene::serializeAudioUnitItems(SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);

    QVariantList list;
    foreach (QGraphicsItem *pItem, items()) {
        if (pItem->type() == SignalChainItem::Type_AudioUnit) {
            SignalChainAudioUnitItem *pSignalChainAuItem = dynamic_cast<SignalChainAudioUnitItem*>(pItem);
            Q_ASSERT(pSignalChainAuItem != nullptr);
            QVariant handle = pContext->serialize(pSignalChainAuItem);
            list.append(handle);
        }
    }

    return list;
}

QVariant SignalChainScene::serializeConnections(SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);

    QVariantList list;
    foreach (QGraphicsItem *pItem, items()) {
        if (pItem->type() == SignalChainItem::Type_Connection) {
            SignalChainConnectionItem *pSignalChainConnectionItem = dynamic_cast<SignalChainConnectionItem*>(pItem);
            Q_ASSERT(pSignalChainConnectionItem != nullptr);

            SignalChainAudioUnitItem *pSourceAu = dynamic_cast<SignalChainAudioUnitItem*>(pSignalChainConnectionItem->outputPortItem()->parentItem());
            SignalChainAudioUnitItem *pTargetAu = dynamic_cast<SignalChainAudioUnitItem*>(pSignalChainConnectionItem->inputPortItem()->parentItem());
            int sourceIndex = pSignalChainConnectionItem->outputPortItem()->outputPort()->index();
            int targetIndex = pSignalChainConnectionItem->inputPortItem()->inputPort()->index();
            Q_ASSERT(sourceIndex >= 0);
            Q_ASSERT(targetIndex >= 0);

            QVariantMap map;
            map["sourceAudioUnit"] = pContext->serialize(pSourceAu);
            map["targetAudioUnit"] = pContext->serialize(pTargetAu);
            map["sourcePortIndex"] = sourceIndex;
            map["targetPortIndex"] = targetIndex;

            list.append(map);
        }
    }

    return list;
}

void SignalChainScene::deserializeAudioUnitItems(const QVariant &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);

    QVariantList list = data.toList();
    foreach (const QVariant &handle, list) {
        SignalChainAudioUnitItem *pAuItem = pContext->deserialize<SignalChainAudioUnitItem>(handle);
        Q_ASSERT(pAuItem != nullptr);
        addItem(pAuItem);
    }
}

void SignalChainScene::deserializeConnections(const QVariant &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);

    QVariantList list = data.toList();
    foreach (const QVariant &vMap, list) {
        QVariantMap map = vMap.toMap();

        SignalChainAudioUnitItem *pSourceAu = pContext->deserialize<SignalChainAudioUnitItem>(map["sourceAudioUnit"]);
        SignalChainAudioUnitItem *pTargetAu = pContext->deserialize<SignalChainAudioUnitItem>(map["targetAudioUnit"]);
        bool ok = false;
        int sourceIndex = map["sourcePortIndex"].toInt(&ok); Q_ASSERT(ok);
        int targetIndex = map["targetPortIndex"].toInt(&ok); Q_ASSERT(ok);
        Q_ASSERT(sourceIndex >= 0);
        Q_ASSERT(targetIndex >= 0);
        SignalChainOutputPortItem *pOutputPortItem = pSourceAu->outputPortItems().at(sourceIndex);
        SignalChainInputPortItem *pInputPortItem = pTargetAu->inputPortItems().at(targetIndex);

        connectPorts(pOutputPortItem, pInputPortItem);
    }
}

QByteArray SignalChainScene::serializeToByteArray(bool selectedOnly) const
{
    SignalChainSceneSelection selection;
    selection.populateFromSignalChainScene(this, selectedOnly);
    if (selection.isEmpty()) {
        return QByteArray();
    }

    SerializationContext context;
    context.serialize(&selection);

    QByteArray data = context.toByteArray();
    return data;
}

void SignalChainScene::deserializeFromByteArray(const QByteArray &data)
{
    if (data.isEmpty()) {
        return;
    }

    SignalChainFactory factory;
    SerializationContext context(&factory);

    context.fromByteArray(data);
    SignalChainSceneSelection *pSelection = context.deserialize<SignalChainSceneSelection>();
    Q_ASSERT(pSelection != nullptr);

    pSelection->putOnScene(this, m_mousePos);
    delete pSelection;
}

SignalChainAudioUnitItem* SignalChainScene::findAudioUnitInstance(const QString &uid)
{
    foreach (QGraphicsItem *pItem, items()) {
        if (pItem->type() == SignalChainItem::Type_AudioUnit) {
            SignalChainAudioUnitItem *pAuItem = dynamic_cast<SignalChainAudioUnitItem*>(pItem);
            if (pAuItem->audioUnit()->uid() == uid) {
                return pAuItem;
            }
        }
    }
    return false;
}
