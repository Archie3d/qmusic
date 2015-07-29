#include <QPainter>
#include <QFont>
#include <QGraphicsSimpleTextItem>
#include "SignalChainAudioUnitItem.h"
#include "SignalChainConnectionItem.h"
#include "SignalChainPortItem.h"

const int cRadius = 4;

QMap<QVariant::Type, QColor> cTypeToColorMap = []() {
    QMap<QVariant::Type, QColor> map;
    map[QVariant::Int] = QColor("blue");
    map[QVariant::Double] = QColor("red");
    map[QVariant::Bool] = QColor("green");
    return map;
}();

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
    QGraphicsSimpleTextItem *pLabelItem = new QGraphicsSimpleTextItem(labelText, this);
    QFont font = pLabelItem->font();
    font.setFamily("Verdana");
    font.setPointSize(7);
    pLabelItem->setFont(font);
    if (type == Type_InputPort) {
        pLabelItem->setPos(-pLabelItem->boundingRect().width() - cRadius, -pLabelItem->boundingRect().height());
    } else {
        pLabelItem->setPos(cRadius, -pLabelItem->boundingRect().height());
    }
}

SignalChainPortItem::~SignalChainPortItem()
{
    foreach (SignalChainConnectionItem *pConnection, m_connections) {
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

void SignalChainPortItem::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
    QVariant::Type type = dataType();
    QColor color = cTypeToColorMap.value(type, Qt::black);

    pPainter->setPen(color);
    pPainter->setBrush(QBrush(Qt::white));
    pPainter->drawPath(path());
}

QVariant SignalChainPortItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemScenePositionHasChanged) {
        foreach (SignalChainConnectionItem *pConnection, m_connections) {
            pConnection->updatePathFromPorts();
        }
    }
    return SignalChainItem::itemChange(change, value);
}

/*
 *  class SignalChainInputPortItem implementation
 */
SignalChainInputPortItem::SignalChainInputPortItem(const InputPortPtr &input, QGraphicsItem *pParent)
    : SignalChainPortItem(Type_InputPort, input->name(), pParent),
      m_inputPortPtr(input)
{
}

/*
 *  class SignalChainOutputPortItem implementation
 */
SignalChainOutputPortItem::SignalChainOutputPortItem(const OutputPortPtr &output, QGraphicsItem *pParent)
    : SignalChainPortItem(Type_OutputPort, output->name(), pParent),
      m_outputPortPtr(output)
{
}
