#include <QDebug>
#include <qmath.h>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QWheelEvent>
#include "SignalChainScene.h"
#include "SignalChainWidget.h"

const qreal cBaseZoomFactor = 1.0015;
const qreal cMinScaleFactor = 0.1;
const qreal cMaxScaleFactor = 4.0;


SignalChainWidget::SignalChainWidget(QWidget *pParent)
    : QWidget(pParent)
{
    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->setMargin(0);
    m_pSignalChainView = new QGraphicsView();
    m_pSignalChainView->setAcceptDrops(true);
    m_pSignalChainView->setRenderHint(QPainter::Antialiasing);
    m_pSignalChainView->setDragMode(QGraphicsView::RubberBandDrag);
    m_pSignalChainView->setMouseTracking(true); // Required to send mouse position changes to the scene.

    m_baseZoomFactor = cBaseZoomFactor;
    m_pSignalChainView->viewport()->installEventFilter(this);

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

void SignalChainWidget::zoom(qreal factor)
{
    QTransform t = m_pSignalChainView->transform();
    qreal minS = factor * qMin(t.m11(), t.m22());
    qreal maxS = factor * qMax(t.m11(), t.m22());
    if (minS < cMinScaleFactor || maxS > cMaxScaleFactor) {
        // Avoid going out of scaling limits
        return;
    }

    m_pSignalChainView->scale(factor, factor);
    m_pSignalChainView->centerOn(m_targetScenePos);
    QPointF deltaViewportPos = m_targetViewportPos - QPointF(m_pSignalChainView->viewport()->width() / 2.0,
                                                             m_pSignalChainView->viewport()->height() / 2.0);
    QPointF viewportCenter = m_pSignalChainView->mapFromScene(m_targetScenePos) - deltaViewportPos;
    m_pSignalChainView->centerOn(m_pSignalChainView->mapToScene(viewportCenter.toPoint()));
}

void SignalChainWidget::resetZoom()
{
    m_pSignalChainView->setTransform(QTransform());
}

bool SignalChainWidget::eventFilter(QObject *pObject, QEvent *pEvent)
{
    Q_UNUSED(pObject);

    if (pEvent->type() == QEvent::MouseMove) {
        QMouseEvent* pMouseEvent = dynamic_cast<QMouseEvent*>(pEvent);
        QPointF delta = m_targetViewportPos - pMouseEvent->pos();
        if (qAbs(delta.x()) > 5 || qAbs(delta.y()) > 5) {
            m_targetViewportPos = pMouseEvent->pos();
            m_targetScenePos = m_pSignalChainView->mapToScene(pMouseEvent->pos());
        }
    } else if (pEvent->type() == QEvent::Wheel) {
        QWheelEvent* pWheelEvent = dynamic_cast<QWheelEvent*>(pEvent);
        if (pWheelEvent->orientation() == Qt::Vertical) {
            double angle = pWheelEvent->angleDelta().y();
            double factor = qPow(m_baseZoomFactor, angle);
            zoom(factor);
            return true;
        }
    } else if (pEvent->type() == QEvent::MouseButtonDblClick) {
        QMouseEvent *pMouseEvent = dynamic_cast<QMouseEvent*>(pEvent);
        if (pMouseEvent->button() == Qt::LeftButton) {
            resetZoom();
        } else {
            m_pSignalChainView->fitInView(m_pSignalChainView->scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
        }
        return true;
    }

    return false;

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

    connect(m_pSignalChainScene, SIGNAL(audioUnitSelected(AudioUnit*)), this, SIGNAL(audioUnitSelected(AudioUnit*)));

    if (pOldScene != nullptr) {
        pOldScene->deleteLater();
    }
}
