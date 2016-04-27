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

#ifndef SIGNALCHAINSCENE_H
#define SIGNALCHAINSCENE_H

#include <QGraphicsScene>
#include "ISerializable.h"
#include "FrameworkApi.h"

class SignalChain;
class SignalChainItem;
class SignalChainPortItem;
class SignalChainOutputPortItem;
class SignalChainInputPortItem;
class SignalChainConnectionItem;
class SignalChainAudioUnitItem;
class AudioUnit;
class AudioUnitPlugin;

/**
 * @brief Graphics scene used to visualize and edit signal chain.
 *
 * The scene handles drag-and-drop operations between the sudio units library
 * and the signal chain canvas.
 */
class QMUSIC_FRAMEWORK_API SignalChainScene : public QGraphicsScene,
                                              public ISerializable
{
    Q_OBJECT

    // Needed to access scene internals to establish units connections
    friend class SignalChainSceneSelection;

public:

    /// Serialization UID.
    const static QString UID;

    SignalChainScene(QObject *pParent = nullptr);
    ~SignalChainScene();

    SignalChain* signalChain() const { return m_pSignalChain; }

    /**
     * @brief Returns signal chain item at given position on scene.
     * @param pos
     * @return Pointer to signal chain item or null if none.
     */
    SignalChainItem* signalChainItemAtPos(const QPointF &pos) const;

    /**
     * Find an existing instance of the audio unit on scene.
     * Only the first found item is returned.
     * @param uid Audio unit UID.
     * @return Poiter to foud item or null of not found.
     */
    SignalChainAudioUnitItem* findAudioUnitInstance(const QString &uid);

    /**
     * Save this scene to a file.
     * @param path File path.
     * @return true if saved OK.
     */
    bool saveToFile(const QString &path);
    static SignalChainScene* loadFromFile(const QString &path);

    // ISerializable interface
    QString uid() const override final { return UID; }
    void serialize(QVariantMap &data, SerializationContext *pContext) const override;
    void deserialize(const QVariantMap &data, SerializationContext *pContext) override;
    static ISerializable* create() { return new SignalChainScene(); }

public slots:

    void selectAll(bool select = true);
    void deleteSelected();
    void deleteAll();

    /**
     * @brief Emable/disable audio units drag operation.
     * Dragging is normally disabled durin the play, so that the items
     * cannot be moved while still being controllable.
     * @param v
     */
    void setAudioUnitsMovable(bool v);

    /**
     * Copy selected items into the clipboard.
     */
    void copyToClipboard();

    /**
     * Paste items from the clipboard at current mouse position.
     */
    void pasteFromClipboard();

signals:

    void beginConnection();
    void endConnection();

    void audioUnitSelected(AudioUnit *pAudioUnit);

protected:

    // Handle UI events (mouse drag-drop and keyboard)

    void mousePressEvent(QGraphicsSceneMouseEvent *pEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *pEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent) override;

    void dragEnterEvent(QGraphicsSceneDragDropEvent *pEvent) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *pEvent) override;
    void dropEvent(QGraphicsSceneDragDropEvent *pEvent) override;

    void keyPressEvent(QKeyEvent *pEvent) override;

    void drawBackground(QPainter *pPainter, const QRectF &rect) override;

private slots:

    /**
     * Handle scene selection change.
     * In case of an audio unit item being selected it will be
     * forwarded to the audioUnitSelected signal used to display
     * the unit's properties.
     */
    void onSelectionChanged();

private:

    /**
     * Connect ports of two audio items.
     * This will also establish connection at signal chain level.
     * @param pOutputPort
     * @param pInputPort
     */
    void connectPorts(SignalChainOutputPortItem *pOutputPort,
                      SignalChainInputPortItem *pInputPort);

    /**
     * Establist the signal ports connection initiated by the mouse drag.
     * @param pFinalPort
     */
    void finilizeConnection(SignalChainPortItem *pFinalPort);

    /**
     * Serialize all audio unit tems of this scene.
     * @param pContext
     * @return
     */
    QVariant serializeAudioUnitItems(SerializationContext *pContext) const;

    /**
     * Serialize all connection on this scene.
     * @param pContext
     * @return
     */
    QVariant serializeConnections(SerializationContext *pContext) const;

    /**
     * Deserialize audio unit items and put them on scene.
     * @param data
     * @param pContext
     */
    void deserializeAudioUnitItems(const QVariant &data, SerializationContext *pContext);

    /**
     * Deserialize and establish connections between audio unit items.
     * @param data
     * @param pContext
     */
    void deserializeConnections(const QVariant &data, SerializationContext *pContext);

    /// Serialize scene selection (copy)
    QByteArray serializeToByteArray(bool selectedOnly) const;

    /// Deserialize scene selection (paste)
    void deserializeFromByteArray(const QByteArray &data);

    /// Signal chain associated to this scene.
    SignalChain *m_pSignalChain;

    AudioUnitPlugin *m_pDraggedAudioUnitPlugin;
    SignalChainConnectionItem *m_pConnectionItem;
    QPointF m_mousePos; ///< Track of mouse position.
};

#endif // AUDIOUNITSSCENE_H

