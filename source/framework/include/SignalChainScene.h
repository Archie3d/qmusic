#ifndef SIGNALCHAINSCENE_H
#define SIGNALCHAINSCENE_H

#include <QGraphicsScene>
#include "FrameworkApi.h"

class SignalChain;
class SignalChainItem;
class SignalChainPortItem;
class SignalChainConnectionItem;
class SignalChainControlItem;
class SignalChainControlConnectionItem;
class AudioUnit;
class AudioUnitPlugin;

class QMUSIC_FRAMEWORK_API SignalChainScene : public QGraphicsScene
{
    Q_OBJECT
public:

    SignalChainScene(QObject *pParent = nullptr);
    ~SignalChainScene();

    SignalChain* signalChain() const { return m_pSignalChain; }

    /**
     * @brief Returns signal chain item at given position on scene.
     * @param pos
     * @return Pointer to signal chain item or null if none.
     */
    SignalChainItem* signalChainItemAtPos(const QPointF &pos) const;

public slots:

    void selectAll();
    void deleteSelected();

signals:

    void beginConnection();
    void endConnection();

    void audioUnitSelected(AudioUnit *pAudioUnit);

protected:

    void mousePressEvent(QGraphicsSceneMouseEvent *pEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *pEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent) override;

    void dragEnterEvent(QGraphicsSceneDragDropEvent *pEvent) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *pEvent) override;
    void dropEvent(QGraphicsSceneDragDropEvent *pEvent) override;

    void keyPressEvent(QKeyEvent *pEvent);

    void drawBackground(QPainter *pPainter, const QRectF &rect) override;

private slots:

    void onSelectionChanged();

private:

    void establishConnection(SignalChainPortItem *pFinalPort);
    void establishControlConnection(SignalChainControlItem *pFinalControl);

    SignalChain *m_pSignalChain;

    AudioUnitPlugin *m_pDraggedAudioUnitPlugin;
    SignalChainConnectionItem *m_pConnectionItem;
    SignalChainControlConnectionItem *m_pControlConnectionItem;
    QPointF m_mousePos; ///< Track of mouse position.
};

#endif // AUDIOUNITSSCENE_H

