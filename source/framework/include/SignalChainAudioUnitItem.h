/*
                          qmusic

    Copyright (C) 2015 Arthur Benilov,
    arthur.benilov@gmail.com

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.
*/

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

/**
 * Graphics items that represents an audio unit on
 * the signal chain scene.
 */
class QMUSIC_FRAMEWORK_API SignalChainAudioUnitItem : public SignalChainItem,
                                                      public ISerializable
{
public:

    /// Serialization UID.
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
    QString uid() const override { return UID; }
    void serialize(QVariantMap &data, SerializationContext *pContext) const override;
    void deserialize(const QVariantMap &data, SerializationContext *pContext) override;
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

