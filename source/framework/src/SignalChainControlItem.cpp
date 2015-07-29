#include <QPainter>
#include <QFont>
#include <QGraphicsSimpleTextItem>
#include "SignalChainAudioUnitItem.h"
#include "SignalChainControlConnectionItem.h"
#include "SignalChainControlItem.h"

const int cSize = 8;

SignalChainControlItem::SignalChainControlItem(Type type, const QString &labelText, QGraphicsItem *pParent)
    : SignalChainItem(type, pParent)
{
    Q_ASSERT(type == Type_ControlInput || type == Type_ControlOutput);

    // Notify when moving ports in order to update connections.
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

    QPainterPath path;
    path.addRect(-cSize/2, -cSize/2, cSize, cSize);
    setPath(path);

    // Add port label
    QGraphicsSimpleTextItem *pLabelItem = new QGraphicsSimpleTextItem(labelText, this);
    QFont font = pLabelItem->font();
    font.setFamily("Verdana");
    font.setPointSize(7);
    pLabelItem->setFont(font);
    if (type == Type_ControlInput) {
        pLabelItem->setPos(cSize/2, cSize/2);
    } else {
        pLabelItem->setPos(cSize/2, -cSize/2 - pLabelItem->boundingRect().height());
    }
}

SignalChainControlItem::~SignalChainControlItem()
{
    // TODO: delete all connections
}

SignalChainAudioUnitItem* SignalChainControlItem::audioUnitItem() const
{
    if (parentItem() != nullptr && parentItem()->type() == Type_AudioUnit) {
        SignalChainAudioUnitItem *pItem = dynamic_cast<SignalChainAudioUnitItem*>(parentItem());
        Q_ASSERT(pItem != nullptr);
        return pItem;
    }
    return nullptr;
}

void SignalChainControlItem::addConnection(SignalChainControlConnectionItem *pConnection)
{
    Q_ASSERT(pConnection != nullptr);

    if (!m_connections.contains(pConnection)) {
        m_connections.append(pConnection);
    }
}

void SignalChainControlItem::removeConnection(SignalChainControlConnectionItem *pConnection)
{
    Q_ASSERT(pConnection != nullptr);
    m_connections.removeOne(pConnection);
    update();
}

void SignalChainControlItem::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
    pPainter->setPen(Qt::blue);
    pPainter->setBrush(QBrush(Qt::white));
    pPainter->drawPath(path());
}

QVariant SignalChainControlItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemScenePositionHasChanged) {
        foreach (SignalChainControlConnectionItem *pConnection, m_connections) {
            pConnection->updatePathFromControls();
        }
    }
    return SignalChainItem::itemChange(change, value);
}

/*
 *  class SignalChainControlInputItem implementation
 */
SignalChainControlInputItem::SignalChainControlInputItem(const ControlInputPortPtr &input, QGraphicsItem *pParent)
    : SignalChainControlItem(Type_ControlInput, input->name(), pParent),
      m_controlInputPtr(input)
{
}

/*
 *  class SignalChainControlOutputItem implementation
 */
SignalChainControlOutputItem::SignalChainControlOutputItem(const ControlOutputPortPtr &output, QGraphicsItem *pParent)
    : SignalChainControlItem(Type_ControlOutput, output->name(), pParent),
      m_controlOutputPtr(output)
{
}
