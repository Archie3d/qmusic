#ifndef SIGNALCHAINAUDIOUNITITEM_H
#define SIGNALCHAINAUDIOUNITITEM_H

#include "FrameworkApi.h"
#include "SignalChainItem.h"

class QGraphicsSimpleTextItem;
class QGraphicsProxyWidget;
class AudioUnit;
class SignalChainConnectionItem;
class SignalChainInputPortItem;
class SignalChainOutputPortItem;

class QMUSIC_FRAMEWORK_API SignalChainAudioUnitItem : public SignalChainItem,
                                                      public ISerializable
{
public:

    const static QString UID;

    SignalChainAudioUnitItem(QGraphicsItem *pParent = nullptr);
    SignalChainAudioUnitItem(AudioUnit *pAudioUnit, QGraphicsItem *pParent = nullptr);
    ~SignalChainAudioUnitItem();

    AudioUnit* audioUnit() const { return m_pAudioUnit; }

    /**
     * @brief Returns list of all connection to/from this unit.
     * @return
     */
    QList<SignalChainConnectionItem*> connectionItems() const;

    const QList<SignalChainInputPortItem*> inputPortItems() const { return m_inputPortItems; }
    const QList<SignalChainOutputPortItem*> outputPortItems() const { return m_outputPortItems; }

    // ISerializable interface
    QString uid() const { return UID; }
    void serialize(QVariantMap &data, SerializationContext *pContext) const;
    void deserialize(const QVariantMap &data, SerializationContext *pContext);
    static ISerializable* create() { return new SignalChainAudioUnitItem(); }

protected:

    void updateView();

    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget) override;

private:

    void init();
    void createDecoration();
    void createPortItems();

    /// Pointer to associated audio unit.
    AudioUnit *m_pAudioUnit;

    QGraphicsItem *m_pAudioUnitGraphicsItem;
    QGraphicsSimpleTextItem *m_pTitleTextItem;

    QList<SignalChainInputPortItem*> m_inputPortItems;
    QList<SignalChainOutputPortItem*> m_outputPortItems;
};

#endif // SIGNALCHAINAUDIOUNITITEM_H

