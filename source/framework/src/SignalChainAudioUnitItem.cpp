#include <QDebug>
#include <QPainter>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsProxyWidget>
#include "AudioUnitPlugin.h"
#include "AudioUnit.h"
#include "SignalChainPortItem.h"
#include "SignalChainConnectionItem.h"
#include "SignalChainAudioUnitItem.h"

const qreal cBorderWidth = 1.0;
const qreal cHeaderMargin = 6.0;
const qreal cPortMargin = 15.0;
const qreal cPortSpacing = 20.0;
const qreal cControlMargin = 15.0;
const qreal cControlSpacing = 40.0;
const QSize cIconSize(16, 16);

SignalChainAudioUnitItem::SignalChainAudioUnitItem(AudioUnit *pAudioUnit, QGraphicsItem *pParent)
    : SignalChainItem(Type_AudioUnit, pParent),
      m_pAudioUnit(pAudioUnit)
{
    Q_ASSERT(pAudioUnit != nullptr);
    createPortItems();

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);

    m_pTitleTextItem = nullptr;
    m_pTitleTextItem = new QGraphicsSimpleTextItem(this);
    m_pTitleTextItem->setText(pAudioUnit->plugin()->name());
    m_pTitleTextItem->setBrush(QBrush(QColor(0, 0, 128)));
    m_pAudioUnitGraphicsItem = m_pAudioUnit->graphicsItem();
    if (m_pAudioUnitGraphicsItem != nullptr) {
        m_pAudioUnitGraphicsItem->setParentItem(this);
    }
}

QList<SignalChainConnectionItem*> SignalChainAudioUnitItem::connectionItems() const
{
    QSet<SignalChainConnectionItem*> connections;

    foreach (SignalChainInputPortItem *pInputItem, m_inputPortItems) {
        foreach (SignalChainConnectionItem *pConnectionItem, pInputItem->connections()) {
            connections.insert(pConnectionItem);
        }
    }

    foreach (SignalChainOutputPortItem *pOutputItem, m_outputPortItems) {
        foreach (SignalChainConnectionItem *pConnectionItem, pOutputItem->connections()) {
            connections.insert(pConnectionItem);
        }
    }

    return connections.toList();
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
        headerRect = m_pTitleTextItem->boundingRect();
        m_pTitleTextItem->setPos(cHeaderMargin*2 + cIconSize.width(), cHeaderMargin);
        headerHeight = qMax(headerRect.height(), (qreal)cIconSize.height());
        headerWidth = headerRect.width() + cHeaderMargin + (qreal)cIconSize.width();
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
    height += contentHeight > 0.0 ? (contentHeight + cHeaderMargin) : 0.0;

    // Reposition ports
    qreal portsLevel = cHeaderMargin + (headerHeight > 0.0 ? (headerHeight + cHeaderMargin) : 0.0);
    qreal step = portsHeight / (nInputs + 1);
    qreal ypos = portsLevel + step;
    foreach (SignalChainInputPortItem *pPortItem, m_inputPortItems) {
        pPortItem->setPos(0, ypos);
        ypos += step;
    }

    step = portsHeight / (nOutputs + 1);
    ypos = portsLevel + step;
    foreach (SignalChainOutputPortItem *pPortItem, m_outputPortItems) {
        pPortItem->setPos(width, ypos);
        ypos += step;
    }


    QSizeF size(width, height);
    QPainterPath path;
    path.addRoundedRect(0, 0, size.width(), size.height(), 3, 3);

    setPath(path);
}

void SignalChainAudioUnitItem::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
    QColor color(220, 220, 220);
    QColor contourColor = color.dark(150);
    QColor colorLight = color.lighter(110);
    QColor colorDark = color.darker(110);
    if (isSelected()) {
        contourColor = QColor(255, 159, 40);
    }

    updateView();

    QRectF rect = boundingRect();
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

    // Draw icon if there is header present
    if (m_pTitleTextItem != nullptr) {
        QPixmap pixmap = m_pAudioUnit->plugin()->icon().pixmap(cIconSize);
        pPainter->drawPixmap(rect.x() + cHeaderMargin, rect.y() + cHeaderMargin,
                             cIconSize.width(), cIconSize.height(),
                             pixmap);
    }
}

void SignalChainAudioUnitItem::createPortItems()
{
    Q_ASSERT(m_pAudioUnit != nullptr);

    foreach (const InputPortPtr &input, m_pAudioUnit->inputs()) {
        SignalChainInputPortItem *pInputItem = new SignalChainInputPortItem(input, this);
        m_inputPortItems.append(pInputItem);
    }

    foreach (const OutputPortPtr &output, m_pAudioUnit->outputs()) {
        SignalChainOutputPortItem *pOutputItem = new SignalChainOutputPortItem(output, this);
        m_outputPortItems.append(pOutputItem);
    }
}
