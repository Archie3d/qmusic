#ifndef SIGNALCHAINAUDIOUNITITEM_H
#define SIGNALCHAINAUDIOUNITITEM_H

#include "FrameworkApi.h"
#include "SignalChainItem.h"

class QGraphicsSimpleTextItem;
class QGraphicsProxyWidget;
class AudioUnit;
class SignalChainConnectionItem;
class SignalChainControlConnectionItem;
class SignalChainInputPortItem;
class SignalChainOutputPortItem;
class SignalChainControlInputItem;
class SignalChainControlOutputItem;

class QMUSIC_FRAMEWORK_API SignalChainAudioUnitItem : public SignalChainItem
{
public:

    SignalChainAudioUnitItem(AudioUnit *pAudioUnit, QGraphicsItem *pParent = nullptr);

    AudioUnit* audioUnit() const { return m_pAudioUnit; }

    /**
     * @brief Returns list of all connection to/from this unit.
     * @return
     */
    QList<SignalChainConnectionItem*> connectionItems() const;

protected:

    void updateView();

    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget) override;

private:

    void createPortItems();

    /// Pointer to associated audio unit.
    AudioUnit *m_pAudioUnit;

    QGraphicsItem *m_pAudioUnitGraphicsItem;
    QGraphicsSimpleTextItem *m_pTitleTextItem;


    QList<SignalChainInputPortItem*> m_inputPortItems;
    QList<SignalChainOutputPortItem*> m_outputPortItems;

    QList<SignalChainControlInputItem*> m_controlInputItems;
    QList<SignalChainControlOutputItem*> m_controlOutputItems;
};

#endif // SIGNALCHAINAUDIOUNITITEM_H

