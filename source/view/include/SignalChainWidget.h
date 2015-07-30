#ifndef SIGNALCHAINVIEW_H
#define SIGNALCHAINVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include "ViewApi.h"

class SignalChainScene;

class QMUSIC_VIEW_API SignalChainWidget : public QWidget
{
    Q_OBJECT

public:

    SignalChainWidget(QWidget *pParent = nullptr);

    SignalChainScene* scene() const { return m_pSignalChainScene; }

    QString sceneFile() const { return m_signalChainSceneFile; }

    void save(const QString &path);

public slots:

    void newSignalChainScene();

private slots:

    void onBeginConnection();
    void onEndConnection();

private:

    QString m_signalChainSceneFile;
    QGraphicsView *m_pSignalChainView;
    SignalChainScene *m_pSignalChainScene;
};

#endif // SIGNALCHAINVIEW_H

