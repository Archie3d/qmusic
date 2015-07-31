#include <QDebug>
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

void SignalChainWidget::save(const QString &path)
{
    if (m_pSignalChainScene->saveToFile(path)) {
        m_signalChainSceneFile = path;
    } else {
        qCritical() << "Unable to save file" << path;
    }
}

void SignalChainWidget::load(const QString &path)
{
    SignalChainScene *pLoadedScene = SignalChainScene::loadFromFile(path);
    if (pLoadedScene != nullptr) {
        setScene(pLoadedScene);
    }
    m_signalChainSceneFile = path;
}

void SignalChainWidget::newSignalChainScene()
{
    setScene(new SignalChainScene());
    m_signalChainSceneFile.clear();
}

void SignalChainWidget::onBeginConnection()
{
    m_pSignalChainView->setDragMode(QGraphicsView::NoDrag);
}

void SignalChainWidget::onEndConnection()
{
    m_pSignalChainView->setDragMode(QGraphicsView::RubberBandDrag);
}

void SignalChainWidget::setScene(SignalChainScene *pScene)
{
    Q_ASSERT(pScene != nullptr);

    SignalChainScene *pOldScene = m_pSignalChainScene;
    m_pSignalChainScene = pScene;

    m_pSignalChainView->setScene(m_pSignalChainScene);
    connect(m_pSignalChainScene, SIGNAL(beginConnection()), this, SLOT(onBeginConnection()));
    connect(m_pSignalChainScene, SIGNAL(endConnection()), this, SLOT(onEndConnection()));

    if (pOldScene != nullptr) {
        pOldScene->deleteLater();
    }
}
