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

    SignalChainWidget(QWidget *pParent = nullptr);

    SignalChainScene* scene() const { return m_pSignalChainScene; }

    QString sceneFile() const { return m_signalChainSceneFile; }

    void save(const QString &path);
    void load(const QString &path);

public slots:

    void newSignalChainScene();

signals:

    void audioUnitSelected(AudioUnit* pAu);

private slots:

    void onBeginConnection();
    void onEndConnection();

private:

    void setScene(SignalChainScene *pScene);

    QString m_signalChainSceneFile;         ///< Saved signal chain file path.
    QGraphicsView *m_pSignalChainView;      ///< Signal chain view.
    SignalChainScene *m_pSignalChainScene;  ///< Signal chain scene.
};

#endif // SIGNALCHAINVIEW_H

