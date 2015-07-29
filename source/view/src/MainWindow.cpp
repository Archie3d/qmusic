#include <QAction>
#include <QToolBar>
#include "LogWindow.h"
#include "AudioUnitsManagerWindow.h"
#include "AudioUnitPropertiesWindow.h"
#include "SignalChainWidget.h"
#include "SignalChainScene.h"
#include "SignalChain.h"
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *pParent, Qt::WindowFlags flags)
    : QMainWindow(pParent, flags)
{
    createDockingWindows();

    createActions();
    createToolBars();

    m_pSignalChainWidget = new SignalChainWidget();
    setCentralWidget(m_pSignalChainWidget);

    connect(m_pSignalChainWidget->scene(), SIGNAL(audioUnitSelected(AudioUnit*)),
            m_pAudioUnitPropertiesWindow, SLOT(handleAudioUnitSelected(AudioUnit*)));

    updateActions();

    resize(1280, 800);
}

MainWindow::~MainWindow()
{
}

void MainWindow::startSignalChain()
{
    m_pSignalChainWidget->scene()->signalChain()->start();
    updateActions();
}

void MainWindow::stopSignalChain()
{
    m_pSignalChainWidget->scene()->signalChain()->stop();
    updateActions();
}

void MainWindow::createDockingWindows()
{
    m_pLogWindow = new LogWindow(this);
    addDockWidget(Qt::BottomDockWidgetArea, m_pLogWindow);

    m_pAudioUnitsManagerWindow = new AudioUnitsManagerWindow(this);
    addDockWidget(Qt::LeftDockWidgetArea, m_pAudioUnitsManagerWindow);

    m_pAudioUnitPropertiesWindow = new AudioUnitPropertiesWindow(this);
    addDockWidget(Qt::RightDockWidgetArea, m_pAudioUnitPropertiesWindow);
}

void MainWindow::createActions()
{
    m_pStartSignalChainAction = new QAction(QIcon(":/icons/play.png"), tr("Start"), this);
    connect(m_pStartSignalChainAction, SIGNAL(triggered()), this, SLOT(startSignalChain()));

    m_pStopSignalChainAction = new QAction(QIcon(":/icons/stop.png"), tr("Stop"), this);
    connect(m_pStopSignalChainAction, SIGNAL(triggered()), this, SLOT(stopSignalChain()));
}

void MainWindow::createToolBars()
{
    m_pSignalChainToolBar = addToolBar(tr("Signal Chain"));
    m_pSignalChainToolBar->setObjectName("signalChainToolBar");
    m_pSignalChainToolBar->addAction(m_pStartSignalChainAction);
    m_pSignalChainToolBar->addAction(m_pStopSignalChainAction);
}

void MainWindow::updateActions()
{
    bool isStarted = m_pSignalChainWidget->scene()->signalChain()->isStarted();
    m_pStartSignalChainAction->setVisible(!isStarted);
    m_pStopSignalChainAction->setVisible(isStarted);
}
