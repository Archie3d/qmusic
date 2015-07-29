#ifndef SIGNALCHAINCONTROLITEM_H
#define SIGNALCHAINCONTROLITEM_H

#include "FrameworkApi.h"
#include "SignalChainItem.h"
#include "ControlInputPort.h"
#include "ControlOutputPort.h"

class SignalChainAudioUnitItem;
class SignalChainControlConnectionItem;

class QMUSIC_FRAMEWORK_API SignalChainControlItem : public SignalChainItem
{
public:

    SignalChainControlItem(Type type, const QString &labelText, QGraphicsItem *pParent = nullptr);
    ~SignalChainControlItem();

    virtual bool isOutput() const = 0;
    bool isInput() const { return !isOutput(); }

    SignalChainAudioUnitItem* audioUnitItem() const;

    void addConnection(SignalChainControlConnectionItem *pConnection);
    void removeConnection(SignalChainControlConnectionItem *pConnection);
    QList<SignalChainControlConnectionItem*> connections() const { return m_connections; }
    bool hasConnections() const { return !m_connections.isEmpty(); }

protected:

    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:

    QList<SignalChainControlConnectionItem*> m_connections;
};

class QMUSIC_FRAMEWORK_API SignalChainControlInputItem : public SignalChainControlItem
{
public:
    SignalChainControlInputItem(const ControlInputPortPtr &input, QGraphicsItem *pParent = nullptr);
    bool isOutput() const override { return false; }
    ControlInputPortPtr controlInput() const { return m_controlInputPtr; }
private:
    ControlInputPortPtr m_controlInputPtr;
};

class QMUSIC_FRAMEWORK_API SignalChainControlOutputItem : public SignalChainControlItem
{
public:
    SignalChainControlOutputItem(const ControlOutputPortPtr &output, QGraphicsItem *pParent = nullptr);
    bool isOutput() const override { return true; }
    ControlOutputPortPtr controlOutput() const { return m_controlOutputPtr; }
private:
    ControlOutputPortPtr m_controlOutputPtr;
};



#endif // SIGNALCHAINCONTROLITEM_H

