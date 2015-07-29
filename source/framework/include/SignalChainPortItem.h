#ifndef SIGNALCHAINPORTITEM_H
#define SIGNALCHAINPORTITEM_H

#include "FrameworkApi.h"
#include "InputPort.h"
#include "OutputPort.h"
#include "SignalChainItem.h"

class SignalChainConnectionItem;
class SignalChainAudioUnitItem;

/**
 * @brief Absttract graphics item representing a signal port.
 */
class QMUSIC_FRAMEWORK_API SignalChainPortItem : public SignalChainItem
{
public:

    SignalChainPortItem(Type type, const QString &labelText, QGraphicsItem *pParent = nullptr);
    ~SignalChainPortItem();

    /**
     * @brief Tells whether this port is an output port.
     * @return true if the port is an output.
     */
    virtual bool isOutput() const = 0;

    bool isInput() const { return !isOutput(); }

    virtual QVariant::Type dataType() const = 0;

    /**
     * @brief Returns pointer to audio unit item this port belongs to.
     * @note Audio unit item is a parent of this port graphics item.
     * @return
     */
    SignalChainAudioUnitItem* audioUnitItem() const;

    void addConnection(SignalChainConnectionItem *pConnection);
    void removeConnection(SignalChainConnectionItem *pConnection);
    QList<SignalChainConnectionItem*> connections() const { return m_connections; }
    bool hasConnections() const { return !m_connections.isEmpty(); }

protected:

    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:

    /// List of connection to this port.
    QList<SignalChainConnectionItem*> m_connections;
};

class QMUSIC_FRAMEWORK_API SignalChainInputPortItem : public SignalChainPortItem
{
public:
    SignalChainInputPortItem(const InputPortPtr &input, QGraphicsItem *pParent = nullptr);
    bool isOutput() const override { return false; }
    QVariant::Type dataType() const override { return m_inputPortPtr->dataType(); }
    InputPortPtr inputPort() const { return m_inputPortPtr; }
private:
    InputPortPtr m_inputPortPtr;
};

class QMUSIC_FRAMEWORK_API SignalChainOutputPortItem : public SignalChainPortItem
{
public:
    SignalChainOutputPortItem(const OutputPortPtr &output, QGraphicsItem *pParent = nullptr);
    bool isOutput() const override { return true; }
    QVariant::Type dataType() const override { return m_outputPortPtr->dataType(); }
    OutputPortPtr outputPort() const { return m_outputPortPtr; }
private:
    OutputPortPtr m_outputPortPtr;
};

#endif // SIGNALCHAINPORTITEM_H

