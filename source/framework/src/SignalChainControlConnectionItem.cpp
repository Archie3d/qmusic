#include <QDebug>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsPathItem>
#include <QStyleOptionGraphicsItem>
#include "ControlInputPort.h"
#include "ControlOutputPort.h"
#include "SignalChainControlItem.h"
#include "SignalChainControlConnectionItem.h"

QColor cCtrlDefaultColor(128, 128, 128);
QColor cCtrlSelectColor(255, 159, 40);
qreal cCtrlWidth(1.5);

SignalChainControlConnectionItem::SignalChainControlConnectionItem(QGraphicsItem *pParent)
    : SignalChainItem(Type_ControlConnection, pParent),
      m_pSourceControlItem(nullptr),
      m_pTargetControlItem(nullptr),
      m_sourcePoint(),
      m_targetPoint()
{
    setZValue(-1);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

SignalChainControlConnectionItem::~SignalChainControlConnectionItem()
{
    if (m_pTargetControlItem != nullptr) {
        m_pTargetControlItem->removeConnection(this);
    }

    if (m_pSourceControlItem != nullptr) {
        ControlOutputPortPtr outputPtr = m_pSourceControlItem->controlOutput();
        if (!outputPtr.isNull()) {
            outputPtr->disconnectAll();
        }
        m_pSourceControlItem->removeConnection(this);
    }
}

void SignalChainControlConnectionItem::setControlSource(SignalChainControlOutputItem *pControlItem)
{
    if (m_pSourceControlItem != nullptr) {
        m_pSourceControlItem->removeConnection(this);
    }
    m_pSourceControlItem = pControlItem;
    if (m_pSourceControlItem != nullptr) {
        m_pSourceControlItem->addConnection(this);
        m_sourcePoint = m_pSourceControlItem->scenePos();
        if (m_pTargetControlItem == nullptr) {
            m_targetPoint = m_sourcePoint;
        }
    }
}

void SignalChainControlConnectionItem::setControlTarget(SignalChainControlInputItem *pControlItem)
{
    if (m_pTargetControlItem != nullptr) {
        m_pTargetControlItem->removeConnection(this);
    }
    m_pTargetControlItem = pControlItem;
    if (m_pTargetControlItem != nullptr) {
        m_pTargetControlItem->addConnection(this);
        m_targetPoint = m_pTargetControlItem->scenePos();
        if (m_pSourceControlItem == nullptr) {
            m_sourcePoint = m_targetPoint;
        }
    }
}

void SignalChainControlConnectionItem::setControl(SignalChainControlItem *pControlItem)
{
    Q_ASSERT(pControlItem != nullptr);

    if (pControlItem->type() == SignalChainItem::Type_ControlInput) {
        setControlTarget(dynamic_cast<SignalChainControlInputItem*>(pControlItem));
    } else if (pControlItem->type() == SignalChainItem::Type_ControlOutput) {
        setControlSource(dynamic_cast<SignalChainControlOutputItem*>(pControlItem));
    }
}

void SignalChainControlConnectionItem::removeControl(SignalChainControlItem *pControlItem)
{
    if (m_pSourceControlItem == dynamic_cast<SignalChainControlOutputItem*>(pControlItem)) {
        m_pSourceControlItem = nullptr;
    }
    if (m_pTargetControlItem == dynamic_cast<SignalChainControlInputItem*>(pControlItem)) {
        m_pTargetControlItem = nullptr;
    }
}

void SignalChainControlConnectionItem::updatePathFromControls()
{
    Q_ASSERT(m_pSourceControlItem != nullptr);
    Q_ASSERT(m_pTargetControlItem != nullptr);

    m_sourcePoint = m_pSourceControlItem->scenePos();
    m_targetPoint = m_pTargetControlItem->scenePos();
    updatePath();
}

void SignalChainControlConnectionItem::updatePath()
{
    // Update connection path
    QPainterPath path;
    path.moveTo(m_sourcePoint);

    qreal dx = m_targetPoint.x() - m_sourcePoint.x();
    qreal dy = m_targetPoint.y() - m_sourcePoint.y();

    // Control points
    QPointF ctrl1(m_sourcePoint.x() + dx * 0.1, m_sourcePoint.y() + dy * 0.25);
    QPointF ctrl2(m_sourcePoint.x() + dx * 0.9, m_sourcePoint.y() + dy * 0.75);

    path.cubicTo(ctrl1, ctrl2, m_targetPoint);

    setPath(path);
    update();

}

bool SignalChainControlConnectionItem::isConnected() const
{
    return (m_pSourceControlItem != nullptr) && (m_pTargetControlItem != nullptr);
}

void SignalChainControlConnectionItem::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
    Q_UNUSED(pWidget);

    QPen pen = isConnected() ? QPen(cCtrlDefaultColor, cCtrlWidth) : QPen(Qt::DotLine);
    if (isConnected() && isSelected()) {
        pen.setColor(cCtrlSelectColor);
    }
    pPainter->setPen(pen);
    pPainter->setBrush(Qt::NoBrush);
    pPainter->drawPath(path());
}
