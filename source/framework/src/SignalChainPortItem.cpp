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
#include <QFont>
#include <QGraphicsSimpleTextItem>
#include "InputPort.h"
#include "OutputPort.h"
#include "SignalChainAudioUnitItem.h"
#include "SignalChainConnectionItem.h"
#include "SignalChainPortItem.h"

const int cRadius = 3;
const QFont cLabelFont("Arial", 9);
const QColor cPortNameColor(132, 180, 50);
const QColor cSocketColor("red");

/*
 * SignalChainPortItem implementation
 */

SignalChainPortItem::SignalChainPortItem(Type type, const QString &labelText, QGraphicsItem *pParent)
    : SignalChainItem(type, pParent),
      m_connections()
{
    Q_ASSERT(type == Type_InputPort || type == Type_OutputPort);

    // Notify when moving ports in order to update connections.
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

    QPainterPath path;
    path.addEllipse(QPointF(0, 0), cRadius, cRadius);
    setPath(path);

    // Add port label
    m_pLabelItem = new QGraphicsSimpleTextItem(this);
    m_pLabelItem->setFont(cLabelFont);
    m_pLabelItem->setText(labelText);
    m_pLabelItem->setBrush(QBrush(cPortNameColor));
    if (type == Type_InputPort) {
        m_pLabelItem->setPos(-m_pLabelItem->boundingRect().width() - cRadius, -m_pLabelItem->boundingRect().height());
    } else {
        m_pLabelItem->setPos(cRadius, -m_pLabelItem->boundingRect().height());
    }
}

SignalChainPortItem::~SignalChainPortItem()
{
    for (SignalChainConnectionItem *pConnection : m_connections) {
        pConnection->removePort(this);
    }
}

SignalChainAudioUnitItem* SignalChainPortItem::audioUnitItem() const
{
    if (parentItem() != nullptr && parentItem()->type() == Type_AudioUnit) {
        SignalChainAudioUnitItem *pItem = dynamic_cast<SignalChainAudioUnitItem*>(parentItem());
        Q_ASSERT(pItem != nullptr);
        return pItem;
    }
    return nullptr;
}

void SignalChainPortItem::addConnection(SignalChainConnectionItem *pConnection)
{
    Q_ASSERT(pConnection != nullptr);

    if (!m_connections.contains(pConnection)) {
        m_connections.append(pConnection);
    }
}

void SignalChainPortItem::removeConnection(SignalChainConnectionItem *pConnection)
{
    Q_ASSERT(pConnection != nullptr);
    m_connections.removeOne(pConnection);
    update();
}

void SignalChainPortItem::setLabel(const QString &text)
{
    m_pLabelItem->setText(text);
    if (type() == Type_InputPort) {
        m_pLabelItem->setPos(-m_pLabelItem->boundingRect().width() - cRadius, -m_pLabelItem->boundingRect().height());
    } else {
        m_pLabelItem->setPos(cRadius, -m_pLabelItem->boundingRect().height());
    }

}

void SignalChainPortItem::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
    pPainter->setPen(cSocketColor);
    pPainter->setBrush(QBrush(Qt::white));
    pPainter->drawPath(path());
}

QVariant SignalChainPortItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemScenePositionHasChanged) {
        for (SignalChainConnectionItem *pConnection : m_connections) {
            pConnection->updatePathFromPorts();
        }
    }
    return SignalChainItem::itemChange(change, value);
}

/*
 *  class SignalChainInputPortItem implementation
 */

SignalChainInputPortItem::SignalChainInputPortItem(QGraphicsItem *pParent)
    : SignalChainPortItem(Type_InputPort, "", pParent),
      m_pInputPort(nullptr)
{
}

SignalChainInputPortItem::SignalChainInputPortItem(InputPort *pInput, QGraphicsItem *pParent)
    : SignalChainPortItem(Type_InputPort, pInput->name(), pParent),
      m_pInputPort(pInput)
{
    Q_ASSERT(pInput != nullptr);
}

/*
 *  class SignalChainOutputPortItem implementation
 */

SignalChainOutputPortItem::SignalChainOutputPortItem(QGraphicsItem *pParent)
    : SignalChainPortItem(Type_OutputPort, "", pParent),
      m_pOutputPort(nullptr)
{

}

SignalChainOutputPortItem::SignalChainOutputPortItem(OutputPort *pOutput, QGraphicsItem *pParent)
    : SignalChainPortItem(Type_OutputPort, pOutput->name(), pParent),
      m_pOutputPort(pOutput)
{
    Q_ASSERT(pOutput != nullptr);
}
