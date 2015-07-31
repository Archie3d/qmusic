#include <QDebug>
#include <QPainter>
#include <QFont>
#include <QGraphicsSimpleTextItem>
#include "InputPort.h"
#include "OutputPort.h"
#include "SerializationContext.h"
#include "SignalChainAudioUnitItem.h"
#include "SignalChainConnectionItem.h"
#include "SignalChainPortItem.h"

const int cRadius = 3;
const int cFontSize = 7;
const char *cFontFamily = "Verdana";

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
        qDebug() << "Width" << m_pLabelItem->boundingRect().width();
        m_pLabelItem->setPos(-m_pLabelItem->boundingRect().width() - cRadius, -m_pLabelItem->boundingRect().height());
    } else {
        m_pLabelItem->setPos(cRadius, -m_pLabelItem->boundingRect().height());
    }

}

void SignalChainPortItem::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);

    // Serialize connection items
    QVariantList list;
    foreach (SignalChainConnectionItem *pItem, m_connections) {
        list.append(pContext->serialize(pItem));
    }

    data["label"] = m_pLabelItem->text();
    data["connections"] = list;
}

void SignalChainPortItem::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);

    m_connections.clear();
    setLabel(data["label"].toString());
    QVariantList list = data["connections"].toList();
    foreach (const QVariant &v, list) {
        auto pItem = pContext->deserialize<SignalChainConnectionItem>(v);
        addConnection(pItem);
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

const QString SignalChainInputPortItem::UID("SignalChainInputPortItem");

SignalChainInputPortItem::SignalChainInputPortItem(QGraphicsItem *pParent)
    : SignalChainPortItem(Type_InputPort, "", pParent),
      m_pInputPort(nullptr)
{
}

SignalChainInputPortItem::SignalChainInputPortItem(InputPort *pInput, QGraphicsItem *pParent)
    : SignalChainPortItem(Type_InputPort, pInput->name(), pParent),
      m_pInputPort(pInput)
{
}

QVariant::Type SignalChainInputPortItem::dataType() const
{
    if (m_pInputPort != nullptr) {
        return m_pInputPort->dataType();
    }

    return QVariant::Invalid;
}

void SignalChainInputPortItem::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    data["inputPort"] = pContext->serialize(m_pInputPort);
    SignalChainPortItem::serialize(data, pContext);
}

void SignalChainInputPortItem::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    m_pInputPort = pContext->deserialize<InputPort>(data["inputPort"]);
    Q_ASSERT(m_pInputPort != nullptr);
    SignalChainPortItem::deserialize(data, pContext);
}

/*
 *  class SignalChainOutputPortItem implementation
 */

const QString SignalChainOutputPortItem::UID("SignalChainOutputPortItem");

SignalChainOutputPortItem::SignalChainOutputPortItem(QGraphicsItem *pParent)
    : SignalChainPortItem(Type_OutputPort, "", pParent),
      m_pOutputPort(nullptr)
{

}

SignalChainOutputPortItem::SignalChainOutputPortItem(OutputPort *pOutput, QGraphicsItem *pParent)
    : SignalChainPortItem(Type_OutputPort, pOutput->name(), pParent),
      m_pOutputPort(pOutput)
{
}

QVariant::Type SignalChainOutputPortItem::dataType() const
{
    if (m_pOutputPort != nullptr) {
        return m_pOutputPort->dataType();
    }

    return QVariant::Invalid;
}

void SignalChainOutputPortItem::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    data["outputPort"] = pContext->serialize(m_pOutputPort);
    SignalChainPortItem::serialize(data, pContext);
}

void SignalChainOutputPortItem::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    m_pOutputPort = pContext->deserialize<OutputPort>(data["outputPort"]);
    Q_ASSERT(m_pOutputPort != nullptr);
    SignalChainPortItem::deserialize(data, pContext);
}
