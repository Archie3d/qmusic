#ifndef SIGNALCHAINCONTROLCONNECTIONITEM_H
#define SIGNALCHAINCONTROLCONNECTIONITEM_H

#include "FrameworkApi.h"
#include "SignalChainItem.h"

class SignalChainControlItem;
class SignalChainControlInputItem;
class SignalChainControlOutputItem;

class QMUSIC_FRAMEWORK_API SignalChainControlConnectionItem : public SignalChainItem
{
public:
    SignalChainControlConnectionItem(QGraphicsItem *pParent = nullptr);
    ~SignalChainControlConnectionItem();

    SignalChainControlInputItem* controlInputItem() const { return m_pTargetControlItem; }
    SignalChainControlOutputItem* controlOutputItem() const { return m_pSourceControlItem; }

    void setControlSource(SignalChainControlOutputItem *pControlItem);
    void setControlTarget(SignalChainControlInputItem *pControlItem);
    void setControl(SignalChainControlItem *pControlItem);
    void removeControl(SignalChainControlItem *pControlItem);

    void setSourcePoint(const QPointF &p) { m_sourcePoint = p; }
    void setTargetPoint(const QPointF &p) { m_targetPoint = p; }
    void updatePathFromControls();
    void updatePath();

    bool isConnected() const;

protected:

    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget);

private:

    SignalChainControlInputItem *m_pTargetControlItem;
    SignalChainControlOutputItem *m_pSourceControlItem;
    QPointF m_sourcePoint;
    QPointF m_targetPoint;
};

#endif // SIGNALCHAINCONTROLCONNECTIONITEM_H

