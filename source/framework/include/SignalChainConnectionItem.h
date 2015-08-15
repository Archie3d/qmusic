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

#ifndef SIGNALCHAINCONNECTIONITEM_H
#define SIGNALCHAINCONNECTIONITEM_H

#include "FrameworkApi.h"
#include "SignalChainItem.h"

class QGraphicsPathItem;
class SignalChainPortItem;
class SignalChainInputPortItem;
class SignalChainOutputPortItem;

/**
 * Graphics items that represents connection between two ports (input and output).
 * It is drawn as cubic spline curve.
 */
class QMUSIC_FRAMEWORK_API SignalChainConnectionItem : public SignalChainItem
{
public:

    SignalChainConnectionItem(QGraphicsItem *pParent = nullptr);
    ~SignalChainConnectionItem();

    SignalChainInputPortItem* inputPortItem() const { return m_pTargetPortItem; }
    SignalChainOutputPortItem* outputPortItem() const { return m_pSourcePortItem; }

    void setSourcePort(SignalChainOutputPortItem *pPort);
    void setTargetPort(SignalChainInputPortItem *pPort);
    void setPort(SignalChainPortItem *pPort);
    void removePort(SignalChainPortItem *pPortItem);

    void setSourcePoint(const QPointF &p) { m_sourcePoint = p; }
    void setTargetPoint(const QPointF &p) { m_targetPoint = p; }
    void updatePathFromPorts();
    void updatePath();

    bool isConnected() const;

protected:

    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget) override;

private:

    SignalChainOutputPortItem *m_pSourcePortItem;
    SignalChainInputPortItem *m_pTargetPortItem;
    QPointF m_sourcePoint;
    QPointF m_targetPoint;
};

#endif // SIGNALCHAINCONNECTIONITEM_H

