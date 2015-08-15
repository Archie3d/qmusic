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

#ifndef SIGNALCHAINVIEW_H
#define SIGNALCHAINVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include "ViewApi.h"

class SignalChainScene;

/**
 * Widget that display the signal chain graphics view.
 */
class QMUSIC_VIEW_API SignalChainWidget : public QWidget
{
    Q_OBJECT

public:

    /**
     * Construct signal chain widget.
     * This will create a new signal chain, its scene and the view.
     * @param pParent
     */
    SignalChainWidget(QWidget *pParent = nullptr);

    SignalChainScene* scene() const { return m_pSignalChainScene; }

    /**
     * Returns name of the file where the scene has been saved or loaded from.
     * @return
     */
    QString sceneFile() const { return m_signalChainSceneFile; }

    /**
     * Save signal chain scene.
     * @param path File path.
     */
    void save(const QString &path);

    /**
     * Load signal chain scene from file.
     * @param path File path.
     */
    void load(const QString &path);

public slots:

    /**
     * Create a new signal chain, and the scene.
     * This will delete currently existing signal chain and the scene.
     */
    void newSignalChainScene();

    /**
     * Zoom the signal chain view.
     * @param factor Zoom factor.
     */
    void zoom(qreal factor);

    /**
     * Reset zoom to 100%.
     */
    void resetZoom();

signals:

    /**
     * Notify audio unit selection.
     * @param pAu Pointer to selected audio unit.
     */
    void audioUnitSelected(AudioUnit* pAu);

protected:

    /**
     * Handle events from the view.
     * @param pObject
     * @param pEvent
     * @return
     */
    bool eventFilter(QObject *pObject, QEvent *pEvent);

private slots:

    void onBeginConnection();
    void onEndConnection();

private:

    void setScene(SignalChainScene *pScene);

    QString m_signalChainSceneFile;         ///< Saved signal chain file path.
    QGraphicsView *m_pSignalChainView;      ///< Signal chain view.
    SignalChainScene *m_pSignalChainScene;  ///< Signal chain scene.

    // Used for zooming
    qreal m_baseZoomFactor;
    QPointF m_targetScenePos;
    QPointF m_targetViewportPos;
};

#endif // SIGNALCHAINVIEW_H

