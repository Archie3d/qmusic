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
const int cFontSize = 7;
const char *cFontFamily = "Verdana";
const QColor cPortNameColor(64, 0, 128);

QMap<QVariant::Type, QColor> cTypeToColorMap = []() {
    QMap<QVariant::Type, QColor> map;
    map[QVariant::Int] = QColor("blue");
    map[QVariant::Double] = QColor("red");
    map[QVariant::Bool] = QColor("green");
    map[QVariant::List] = QColor("cyan");
    map[QVariant::Map] = QColor("magenta");
    return map;
}();

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
    QFont font = m_pLabelItem->font();
    font.setFamily(cFontFamily);
    font.setPointSize(cFontSize);
    m_pLabelItem->setFont(font);
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

QVariant::Type SignalChainInputPortItem::dataType() const
{
    if (m_pInputPort != nullptr) {
        return m_pInputPort->dataType();
    }

    return QVariant::Invalid;
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

QVariant::Type SignalChainOutputPortItem::dataType() const
{
    if (m_pOutputPort != nullptr) {
        return m_pOutputPort->dataType();
    }

    return QVariant::Invalid;
}
