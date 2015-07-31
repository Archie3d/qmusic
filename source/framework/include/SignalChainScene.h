#ifndef SIGNALCHAINSCENE_H
#define SIGNALCHAINSCENE_H

#include <QGraphicsScene>
#include "ISerializable.h"
#include "FrameworkApi.h"

class SignalChain;
class SignalChainItem;
class SignalChainPortItem;
class SignalChainConnectionItem;
class AudioUnit;
class AudioUnitPlugin;

class QMUSIC_FRAMEWORK_API SignalChainScene : public QGraphicsScene,
                                              public ISerializable
{
    Q_OBJECT
public:

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
     * Save this scene to a file.
     * @param path File path.
     * @return true if saved OK.
     */
    bool saveToFile(const QString &path);
    static SignalChainScene* loadFromFile(const QString &path);

    // ISerializable interface
    QString uid() const override final { return UID; }
    void serialize(QVariantMap &data, SerializationContext *pContext) const;
    void deserialize(const QVariantMap &data, SerializationContext *pContext);
    static ISerializable* create() { return new SignalChainScene(); }

public slots:

    void selectAll();
    void deleteSelected();
    void deleteAll();

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

    SignalChain *m_pSignalChain;

    AudioUnitPlugin *m_pDraggedAudioUnitPlugin;
    SignalChainConnectionItem *m_pConnectionItem;
    QPointF m_mousePos; ///< Track of mouse position.
};

#endif // AUDIOUNITSSCENE_H

