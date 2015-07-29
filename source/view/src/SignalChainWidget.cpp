#include <QVBoxLayout>
#include "SignalChainScene.h"
#include "SignalChainWidget.h"

SignalChainWidget::SignalChainWidget(QWidget *pParent)
    : QWidget(pParent)
{
    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->setMargin(0);
    m_pSignalChainView = new QGraphicsView();
    m_pSignalChainView->setAcceptDrops(true);
    m_pSignalChainView->setRenderHint(QPainter::Antialiasing);
    m_pSignalChainView->setDragMode(QGraphicsView::RubberBandDrag);

    pLayout->addWidget(m_pSignalChainView);
    setLayout(pLayout);

    m_pSignalChainScene = nullptr;
    newSignalChainScene();
}

void SignalChainWidget::newSignalChainScene()
{
    SignalChainScene *pOldScene = m_pSignalChainScene;
    m_pSignalChainScene = new SignalChainScene();
    m_pSignalChainView->setScene(m_pSignalChainScene);
    connect(m_pSignalChainScene, SIGNAL(beginConnection()), this, SLOT(onBeginConnection()));
    connect(m_pSignalChainScene, SIGNAL(endConnection()), this, SLOT(onEndConnection()));
    if (pOldScene != nullptr) {
        pOldScene->deleteLater();
    }
}

void SignalChainWidget::onBeginConnection()
{
    m_pSignalChainView->setDragMode(QGraphicsView::NoDrag);
}

void SignalChainWidget::onEndConnection()
{
    m_pSignalChainView->setDragMode(QGraphicsView::RubberBandDrag);
}
