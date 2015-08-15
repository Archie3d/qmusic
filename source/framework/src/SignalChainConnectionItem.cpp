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
#include <QGraphicsScene>
#include <QGraphicsPathItem>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsDropShadowEffect>
#include "InputPort.h"
#include "OutputPort.h"
#include "SerializationContext.h"
#include "SignalChainPortItem.h"
#include "SignalChainConnectionItem.h"

QColor cDefaultColor(64, 64, 64);
QColor cSelectColor(255, 159, 40);
qreal cWidth(1.25);

SignalChainConnectionItem::SignalChainConnectionItem(QGraphicsItem *pParent)
    : SignalChainItem(Type_Connection, pParent),
      m_pSourcePortItem(nullptr),
      m_pTargetPortItem(nullptr),
      m_sourcePoint(),
      m_targetPoint()
{
    setZValue(-1);

    setFlag(QGraphicsItem::ItemIsSelectable);

    QGraphicsDropShadowEffect *pShadowEffect = new QGraphicsDropShadowEffect();
    pShadowEffect->setOffset(3.0);
    pShadowEffect->setBlurRadius(15.0);
    setGraphicsEffect(pShadowEffect);
}

SignalChainConnectionItem::~SignalChainConnectionItem()
{
    if (m_pTargetPortItem != nullptr) {
        InputPort *pInputPort = m_pTargetPortItem->inputPort();
        if (pInputPort != nullptr) {
            pInputPort->disconnect();
        }
        m_pTargetPortItem->removeConnection(this);
    }

    if (m_pSourcePortItem != nullptr) {
        m_pSourcePortItem->removeConnection(this);
    }

}

void SignalChainConnectionItem::setSourcePort(SignalChainOutputPortItem *pPort)
{
    if (m_pSourcePortItem != nullptr) {
        m_pSourcePortItem->removeConnection(this);
    }
    m_pSourcePortItem = pPort;
    if (m_pSourcePortItem != nullptr) {
        m_pSourcePortItem->addConnection(this);
        m_sourcePoint = m_pSourcePortItem->scenePos();
        if (m_pTargetPortItem == nullptr) {
            m_targetPoint = m_sourcePoint;
        }
    }
}

void SignalChainConnectionItem::setTargetPort(SignalChainInputPortItem *pPort)
{
    if (m_pTargetPortItem != nullptr) {
        m_pTargetPortItem->removeConnection(this);
    }
    m_pTargetPortItem = pPort;
    if (m_pTargetPortItem != nullptr) {
        m_pTargetPortItem->addConnection(this);
        m_targetPoint = m_pTargetPortItem->scenePos();
        if (m_pSourcePortItem == nullptr) {
            m_sourcePoint = m_targetPoint;
        }
    }
}

void SignalChainConnectionItem::setPort(SignalChainPortItem *pPort)
{
    Q_ASSERT(pPort != nullptr);

    if (pPort->type() == SignalChainItem::Type_InputPort) {
        setTargetPort(dynamic_cast<SignalChainInputPortItem*>(pPort));
    } else if (pPort->type() == SignalChainItem::Type_OutputPort) {
        setSourcePort(dynamic_cast<SignalChainOutputPortItem*>(pPort));
    }
}

void SignalChainConnectionItem::removePort(SignalChainPortItem *pPortItem)
{    
    // This method may be called by destructor of the port item.
    // dynamic cast must not be used here.
    if (m_pSourcePortItem == static_cast<SignalChainOutputPortItem*>(pPortItem)) {
        m_pSourcePortItem = nullptr;
    }
    if (m_pTargetPortItem == static_cast<SignalChainInputPortItem*>(pPortItem)) {
        m_pTargetPortItem = nullptr;
    }
}

void SignalChainConnectionItem::updatePathFromPorts()
{
    Q_ASSERT(m_pSourcePortItem != nullptr);
    Q_ASSERT(m_pTargetPortItem != nullptr);

    m_sourcePoint = m_pSourcePortItem->scenePos();
    m_targetPoint = m_pTargetPortItem->scenePos();
    updatePath();
}

void SignalChainConnectionItem::updatePath()
{
    // Update connection path
    QPainterPath path;
    path.moveTo(m_sourcePoint);

    qreal dx = m_targetPoint.x() - m_sourcePoint.x();
    qreal dy = m_targetPoint.y() - m_sourcePoint.y();

    // Control points
    QPointF ctrl1(m_sourcePoint.x() + dx * 0.25, m_sourcePoint.y() + dy * 0.1);
    QPointF ctrl2(m_sourcePoint.x() + dx * 0.75, m_sourcePoint.y() + dy * 0.9);

    path.cubicTo(ctrl1, ctrl2, m_targetPoint);

    setPath(path);
    update();
}

bool SignalChainConnectionItem::isConnected() const
{
    return (m_pSourcePortItem != nullptr) && (m_pTargetPortItem != nullptr);
}

void SignalChainConnectionItem::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
    Q_UNUSED(pWidget);

    QPen pen = isConnected() ? QPen(cDefaultColor, cWidth) : QPen(Qt::DotLine);
    if (isConnected() && isSelected()) {
        pen.setColor(cSelectColor);
    }
    pPainter->setPen(pen);
    pPainter->setBrush(Qt::NoBrush);
    pPainter->drawPath(path());
}
