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
#include <QGraphicsSimpleTextItem>
#include <QGraphicsDropShadowEffect>
#include "SerializationContext.h"
#include "AudioUnitPlugin.h"
#include "AudioUnit.h"
#include "SignalChainPortItem.h"
#include "SignalChainConnectionItem.h"
#include "SignalChainAudioUnitItem.h"

const qreal cBorderWidth = 1.0;
const qreal cHeaderMargin = 3.0;
const qreal cPortMargin = 5.0;
const qreal cPortSpacing = 20.0;
const QSize cIconSize(16, 16);

const QColor cTitleColor(92, 170, 200);
const QColor cSelectionColor(255, 159, 40);

const QString SignalChainAudioUnitItem::UID("SignalChainAudioUnitItem");

SignalChainAudioUnitItem::SignalChainAudioUnitItem(QGraphicsItem *pParent)
    : SignalChainItem(Type_AudioUnit, pParent)
{
    init();
}

SignalChainAudioUnitItem::SignalChainAudioUnitItem(AudioUnit *pAudioUnit, QGraphicsItem *pParent)
    : SignalChainItem(Type_AudioUnit, pParent),
      m_pAudioUnit(pAudioUnit)
{
    Q_ASSERT(pAudioUnit != nullptr);
    createPortItems();

    init();

    createDecoration();
}

SignalChainAudioUnitItem::~SignalChainAudioUnitItem()
{
    delete m_pAudioUnit;
}

QList<SignalChainConnectionItem*> SignalChainAudioUnitItem::connectionItems() const
{
    QSet<SignalChainConnectionItem*> connections;

    for (SignalChainInputPortItem *pInputItem : m_inputPortItems) {
        for (SignalChainConnectionItem *pConnectionItem : pInputItem->connections()) {
            connections.insert(pConnectionItem);
        }
    }

    for (SignalChainOutputPortItem *pOutputItem : m_outputPortItems) {
        for (SignalChainConnectionItem *pConnectionItem : pOutputItem->connections()) {
            connections.insert(pConnectionItem);
        }
    }

    return connections.toList();
}

void SignalChainAudioUnitItem::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    data["audioUnit"] = pContext->serialize(m_pAudioUnit);
    data["position"] = pos();
}

void SignalChainAudioUnitItem::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);

    ISerializable *pSerializable = pContext->deserialize(data["audioUnit"]);
    m_pAudioUnit = dynamic_cast<AudioUnit*>(pSerializable);
    Q_ASSERT(m_pAudioUnit);

    // Restore item's position
    QPointF pos = data["position"].toPointF();
    setPos(pos);

    // Recreate title and graphics item
    delete m_pTitleTextItem;
    delete m_pAudioUnitGraphicsItem;
    m_pTitleTextItem = nullptr;
    m_pAudioUnitGraphicsItem = nullptr;

    createPortItems();
    createDecoration();
}

void SignalChainAudioUnitItem::updateView()
{
    if (scene() == nullptr) {
        // Item is not on scene
        return;
    }

    QRectF headerRect;
    qreal headerHeight = 0.0;
    qreal headerWidth = 0.0;
    if (m_pTitleTextItem != nullptr) {
        m_pTitleTextItem->setText(m_pAudioUnit->title());
        headerRect = m_pTitleTextItem->boundingRect();
        QSize iconSize(0, 0);
        if ((m_pAudioUnit->flags() & IAudioUnit::Flag_NoIcon) == 0) {
            iconSize = cIconSize;
        }
        m_pTitleTextItem->setPos(cHeaderMargin*2 + iconSize.width(), cHeaderMargin);
        headerHeight = qMax(headerRect.height(), (qreal)iconSize.height());
        headerWidth = headerRect.width() + cHeaderMargin + (qreal)iconSize.width();
    }

    qreal contentWidth = 0.0;
    qreal contentHeight = 0.0;

    if (m_pAudioUnitGraphicsItem != nullptr) {
        qreal ypos = (headerHeight > 0.0 ? (headerHeight + cHeaderMargin) : 0.0) + cHeaderMargin;
        m_pAudioUnitGraphicsItem->setPos(cHeaderMargin, ypos);
        contentWidth = m_pAudioUnitGraphicsItem->boundingRect().width();
        contentHeight = m_pAudioUnitGraphicsItem->boundingRect().height();
    }

    int nInputs = m_pAudioUnit->inputs().count();
    int nOutputs = m_pAudioUnit->outputs().count();

    qreal inputsHeight = nInputs * cPortSpacing;
    qreal outputsHeight = nOutputs * cPortSpacing;
    qreal portsHeight = qMax(inputsHeight, outputsHeight);

    portsHeight = qMax(portsHeight, contentHeight);
    contentHeight = qMax(contentHeight, portsHeight);

    qreal width = qMax(headerWidth, contentWidth) + 2*cHeaderMargin;
    qreal height = cHeaderMargin + (headerHeight > 0.0 ? (headerHeight + cHeaderMargin) : 0.0);

    height += contentHeight;

    qreal portsLevel = cHeaderMargin + (headerHeight > 0.0 ? (headerHeight + cHeaderMargin) : 0.0);

    // Reposition ports
    if (nInputs > 0) {
        qreal step = portsHeight / nInputs;
        qreal ypos = portsLevel + step / 2;
        for (SignalChainInputPortItem *pPortItem : m_inputPortItems) {
            pPortItem->setPos(0, ypos);
            ypos += step;
        }
    }

    if (nOutputs > 0) {
        qreal step = portsHeight / nOutputs;
        qreal ypos = portsLevel + step / 2;
        for (SignalChainOutputPortItem *pPortItem : m_outputPortItems) {
            pPortItem->setPos(width, ypos);
            ypos += step;
        }
    }

    QSizeF size(width, height);
    QPainterPath path;
    path.addRoundedRect(0, 0, size.width(), size.height(), 3, 3);

    setPath(path);

    // Drop shadow effect if not yet created
    if (graphicsEffect() == nullptr) {
        QGraphicsDropShadowEffect *pShadowEffect = new QGraphicsDropShadowEffect();
        pShadowEffect->setOffset(0.0);
        pShadowEffect->setBlurRadius(15.0);
        setGraphicsEffect(pShadowEffect);
    }
}

void SignalChainAudioUnitItem::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
    Q_UNUSED(pOption);
    Q_UNUSED(pWidget);

    updateView();

    QRectF rect = boundingRect();

    if ((m_pAudioUnit->flags() & IAudioUnit::Flag_NoFrame) == 0) {
        // Draw frame

        QColor color = m_pAudioUnit->color();
        QColor contourColor = color.dark(150);
        QColor colorLight = color.lighter(110);
        QColor colorDark = color.darker(110);
        if (isSelected()) {
            contourColor = cSelectionColor;
        }

        QLinearGradient gradient(QPointF(0, rect.top()),
                                 QPointF(0, rect.bottom()));
        gradient.setColorAt(0, colorLight);
        gradient.setColorAt(1, colorDark);

        QPen pen;
        pen.setColor(contourColor);
        pen.setWidthF(cBorderWidth);
        pPainter->setPen(pen);
        pPainter->setBrush(gradient);

        pPainter->drawPath(path());
    } else {
        if (isSelected()) {
            // Draw selection frame
            QPen pen;
            pen.setColor(cSelectionColor);
            pen.setStyle(Qt::DashLine);
            pPainter->setPen(pen);
            pPainter->drawPath(path());
        }
    }

    // Draw icon if there is header present
    if (m_pTitleTextItem != nullptr) {
        // But only if not disabled
        if ((m_pAudioUnit->flags() & IAudioUnit::Flag_NoIcon) == 0) {
            QPixmap pixmap = m_pAudioUnit->plugin()->icon().pixmap(cIconSize);
            pPainter->drawPixmap(rect.x() + cHeaderMargin, rect.y() + cHeaderMargin,
                                 cIconSize.width(), cIconSize.height(),
                                 pixmap);
        }
    }
}

void SignalChainAudioUnitItem::init()
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);

    m_pTitleTextItem = nullptr;
    m_pAudioUnitGraphicsItem = nullptr;
}

void SignalChainAudioUnitItem::createDecoration()
{
    Q_ASSERT(m_pTitleTextItem == nullptr);
    Q_ASSERT(m_pAudioUnitGraphicsItem == nullptr);
    Q_ASSERT(m_pAudioUnit != nullptr);

    if ((m_pAudioUnit->flags() & IAudioUnit::Flag_NoTitle) == 0) {
        m_pTitleTextItem = new QGraphicsSimpleTextItem(this);
        m_pTitleTextItem->setText(m_pAudioUnit->title());
        m_pTitleTextItem->setBrush(QBrush(cTitleColor));
    }

    m_pAudioUnitGraphicsItem = m_pAudioUnit->graphicsItem();
    if (m_pAudioUnitGraphicsItem != nullptr) {
        m_pAudioUnitGraphicsItem->setParentItem(this);
    }
}

void SignalChainAudioUnitItem::createPortItems()
{
    Q_ASSERT(m_pAudioUnit != nullptr);

    for (InputPort *pInput : m_pAudioUnit->inputs()) {
        SignalChainInputPortItem *pInputItem = new SignalChainInputPortItem(pInput, this);
        m_inputPortItems.append(pInputItem);
    }

    for (OutputPort *pOutput : m_pAudioUnit->outputs()) {
        SignalChainOutputPortItem *pOutputItem = new SignalChainOutputPortItem(pOutput, this);
        m_outputPortItems.append(pOutputItem);
    }
}
