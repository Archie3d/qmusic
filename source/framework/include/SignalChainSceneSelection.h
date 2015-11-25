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

#ifndef SIGNALCHAINSCENESELECTION_H
#define SIGNALCHAINSCENESELECTION_H

#include "FrameworkApi.h"
#include "ISerializable.h"

class SignalChainScene;
class SignalChainAudioUnitItem;
class SignalChainConnectionItem;

/**
 * @brief Container for signal chain selected items.
 *
 * This class handles partial selection of audio unit items on the signal chain scene.
 * It is used for copy and paste operations.
 */
class QMUSIC_FRAMEWORK_API SignalChainSceneSelection : public ISerializable
{
public:

    /// Serialization UID.
    static const QString UID;

    struct ConnectionStruct
    {
        SignalChainAudioUnitItem *pSourceAudioUnitItem;
        SignalChainAudioUnitItem *pTargetAudioUnitItem;
        int sourcePortIndex;
        int targetPortIndex;
    };

    SignalChainSceneSelection();

    /**
     * Populate this selection from the signal chain scene.
     * @param pScene Pointer to the signal chain scene.
     * @param selectedOnly true if only selected items to be picked.
     */
    void populateFromSignalChainScene(const SignalChainScene *pScene, bool selectedOnly = true);

    /**
     * Put audio unit and connection items from this selection onto the signal chain scene.
     * @param pScene
     * @param pos Position where the items should be put.
     */
    void putOnScene(SignalChainScene *pScene, const QPointF &pos = QPointF());

    /**
     * Tells whether this selection is empty.
     * @return
     */
    bool isEmpty() const;

    // ISerializable interface
    QString uid() const override final { return UID; }
    void serialize(QVariantMap &data, SerializationContext *pContext) const;
    void deserialize(const QVariantMap &data, SerializationContext *pContext);

    static ISerializable* create() { return new SignalChainSceneSelection(); }

private:

    /// Remove all items from this selection.
    void clear();

    QList<SignalChainAudioUnitItem*> m_audioUnitItems;  ///< List of selected audio unit items.
    QList<ConnectionStruct> m_connections;              ///< List of selected connection items.
};

#endif // SIGNALCHAINSCENESELECTION_H

