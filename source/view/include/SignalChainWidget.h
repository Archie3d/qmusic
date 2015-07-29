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

public slots:

    void newSignalChainScene();

private slots:

    void onBeginConnection();
    void onEndConnection();

private:

    QGraphicsView *m_pSignalChainView;
    SignalChainScene *m_pSignalChainScene;
};

#endif // SIGNALCHAINVIEW_H

