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

#include <QDebug>
#include <QSettings>
#include <QAction>
#include <QCloseEvent>
#include <QMenuBar>
#include <QToolBar>
#include <QProgressBar>
#include <QLabel>
#include <QStyle>
#include <QMessageBox>
#include <QFileDialog>
#include "Application.h"
#include "Settings.h"
#include "LogWindow.h"
#include "AudioDevicesManager.h"
#include "AudioUnitsManagerWindow.h"
#include "AudioUnitPropertiesWindow.h"
#include "PianoKeyboardWindow.h"
#include "SpectrumWindow.h"
#include "SignalChainWidget.h"
#include "SignalChainScene.h"
#include "SignalChain.h"
#include "IEventRouter.h"
#include "SettingsDialog.h"
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *pParent, Qt::WindowFlags flags)
    : QMainWindow(pParent, flags)
{
    setObjectName("mainWindow");
    setWindowIcon(QIcon(":/icons/qmusic.png"));

    createDockingWindows();

    createActions();
    createMenu();
    createToolBars();

    m_pSignalChainWidget = new SignalChainWidget();
    setCentralWidget(m_pSignalChainWidget);

    connect(m_pSignalChainWidget, SIGNAL(audioUnitSelected(AudioUnit*)),
            m_pAudioUnitPropertiesWindow, SLOT(handleAudioUnitSelected(AudioUnit*)));

    // Open settings dialog when audio manager reports devices not being configured properly
    connect(Application::instance()->audioDevicesManager(), SIGNAL(devicesNotConfigured()),
            m_pSettingsAction, SLOT(trigger()));

    // Set default size used at the first start only.
    // This will be overridden by saved settings.
    resize(1280, 900);

    loadSettings();

    updateActions();    

    logInfo(tr("*** <b>%1</b> version %2 ***")
            .arg(Application::Product)
            .arg(QMUSIC_VERSION));
}

MainWindow::~MainWindow()
{
}

void MainWindow::updateDspLoad(float l)
{
    if (m_pSignalChainWidget->scene()->signalChain()->isStarted()) {
        //int value = (m_pDspLoadBar->value()*0.9  + m_pDspLoadBar->maximum() * l * 0.1);

        if (l < 0.5f) {
            m_pDspLoadBar->setProperty("loadLevel", 0);
        } else if (l < 0.8f) {
            m_pDspLoadBar->setProperty("loadLevel", 1);
        } else {
            m_pDspLoadBar->setProperty("loadLevel", 2);
        }

        l = qMin(l, 1.0f);

        int value = m_pDspLoadBar->maximum() * l;
        m_pDspLoadBar->setValue(value);
        m_pDspLoadBar->style()->unpolish(m_pDspLoadBar);
        m_pDspLoadBar->style()->polish(m_pDspLoadBar);
        m_pDspLoadBar->update();
    }
}

void MainWindow::closeEvent(QCloseEvent *pEvent)
{
    int ret = QMessageBox::question(
                this,
                tr("Quit"),
                tr("Do you want to exit the application<br>"
                   "(all unsaved data will be lost)?"),
                QMessageBox::Yes |
                QMessageBox::No,
                QMessageBox::No);

    if (ret == QMessageBox::No) {
        pEvent->ignore();
        return;
    }

    stopSignalChain();

    saveSettings();

    QMainWindow::closeEvent(pEvent);
}

void MainWindow::newSignalChain()
{
    int ret = QMessageBox::question(
                this,
                tr("New signal chain"),
                tr("Do you want to create a new signal chain?<br>"
                   "(All unsaved data will be lost)"),
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        m_pSignalChainWidget->newSignalChainScene();
        setWindowTitle("New signal chain");
        logInfo(tr("New signal chain created"));
    }
}

void MainWindow::saveSignalChain()
{
    if (m_pSignalChainWidget->sceneFile().isEmpty()) {
        saveAsSignalChain();
        return;
    }

    m_pSignalChainWidget->save(m_pSignalChainWidget->sceneFile());
    setWindowTitle(m_pSignalChainWidget->sceneFile());
}

void MainWindow::saveAsSignalChain()
{
    QString proposedPath = Application::instance()->applicationDirPath();
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save signal chain as"),
                                                    proposedPath,
                                                    tr("QMusic signal chain (*.sch)"));
    if (fileName.isEmpty()) {
        return;
    }

    m_pSignalChainWidget->save(fileName);
    logInfo(tr("Signal chain saved as %1").arg(fileName));
    setWindowTitle(fileName);
}

void MainWindow::openSignalChain()
{
    QString proposedPath = Application::instance()->applicationDirPath();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open signal chain"),
                                                    proposedPath,
                                                    tr("QMusic signalchain (*.sch)"));

    if (fileName.isEmpty()) {
        return;
    }

    m_pSignalChainWidget->load(fileName);
    setWindowTitle(fileName);

    logInfo(tr("Loaded signal chain from %1").arg(fileName));
}

void MainWindow::startSignalChain()
{
    // Update sample rate
    Settings settings;
    double sampleRate = settings.get(Settings::Setting_SampleRate).toDouble();
    m_pSignalChainWidget->scene()->signalChain()->setTimeStep(1.0 / sampleRate);

    // Start audio devices
    Application::instance()->audioDevicesManager()->startAudioDevices();
    if (!Application::instance()->audioDevicesManager()->isStarted()) {
        // Unable to start audio interfaces, cancel the sequence
        return;
    }

    // Subscribe signal chain for events
    Application::instance()->eventRouter()->registerHandler(m_pSignalChainWidget->scene()->signalChain());

    // Start signal chain
    m_pSignalChainWidget->scene()->setAudioUnitsMovable(false);
    m_pSignalChainWidget->scene()->signalChain()->start();
    m_pSignalChainWidget->scene()->signalChain()->enable(true); // Enable signal chain by default
    updateActions();
    logInfo(tr("Synthesizer started"));
}

void MainWindow::stopSignalChain()
{
    // Unregister signal chain and purge event router
    Application::instance()->eventRouter()->unregisterHandler(m_pSignalChainWidget->scene()->signalChain());

    SignalChain *pSignalChain = m_pSignalChainWidget->scene()->signalChain();
    Q_ASSERT(pSignalChain != nullptr);

    pSignalChain->stop();
    pSignalChain->enable(false); // Disable signal chain
    m_pSignalChainWidget->scene()->setAudioUnitsMovable(true);
    Application::instance()->audioDevicesManager()->stopAudioDevices();

    // Purge event router
    Application::instance()->eventRouter()->purge();

    // Clear piano keyboard
    m_pPianoKeyboardWindow->reset();

#ifdef PROFILING
    pSignalChain->profilingLog();
#endif // PROFILING

    updateActions();
    m_pDspLoadBar->setValue(0);
    m_pSpectrumWindow->reset();
    logInfo(tr("Synthesizer stopped"));
}

void MainWindow::editSettings()
{
    SettingsDialog settingsDialog;

    settingsDialog.loadSettings();

    if (settingsDialog.exec() == QDialog::Accepted) {
        settingsDialog.saveSettings();
    }
}

void MainWindow::createDockingWindows()
{
    m_pLogWindow = new LogWindow(this);
    addDockWidget(Qt::BottomDockWidgetArea, m_pLogWindow);

    m_pPianoKeyboardWindow = new PianoKeyboardWindow(this);
    Application::instance()->eventRouter()->registerHandler(m_pPianoKeyboardWindow);
    addDockWidget(Qt::BottomDockWidgetArea, m_pPianoKeyboardWindow);
    tabifyDockWidget(m_pLogWindow, m_pPianoKeyboardWindow);

    m_pAudioUnitsManagerWindow = new AudioUnitsManagerWindow(this);
    addDockWidget(Qt::LeftDockWidgetArea, m_pAudioUnitsManagerWindow);

    m_pSpectrumWindow = new SpectrumWindow(this);
    addDockWidget(Qt::TopDockWidgetArea, m_pSpectrumWindow);

    m_pAudioUnitPropertiesWindow = new AudioUnitPropertiesWindow(this);
    addDockWidget(Qt::RightDockWidgetArea, m_pAudioUnitPropertiesWindow);

    // Force log window to be displayed by default
    m_pLogWindow->raise();
}

void MainWindow::createActions()
{
    m_pNewSignalChainAction = new QAction(QIcon(":/icons/new.png"), tr("&New"), this);
    m_pNewSignalChainAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    connect(m_pNewSignalChainAction, SIGNAL(triggered()), this, SLOT(newSignalChain()));

    m_pOpenSignalChainAction = new QAction(QIcon(":/icons/open.png"), tr("&Open..."), this);
    connect(m_pOpenSignalChainAction, SIGNAL(triggered()), this, SLOT(openSignalChain()));

    m_pSaveSignalChainAction = new QAction(QIcon(":/icons/save.png"), tr("&Save"), this);
    m_pSaveSignalChainAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    connect(m_pSaveSignalChainAction, SIGNAL(triggered()), this, SLOT(saveSignalChain()));

    m_pSaveAsSignalChainAction = new QAction(tr("Save &as..."), this);
    connect(m_pSaveAsSignalChainAction, SIGNAL(triggered()), this, SLOT(saveAsSignalChain()));

    m_pQuitAction = new QAction(tr("&Quit"), this);
    m_pQuitAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    connect(m_pQuitAction, SIGNAL(triggered()), this, SLOT(close()));

    m_pStartSignalChainAction = new QAction(QIcon(":/icons/play.png"), tr("Start"), this);
    connect(m_pStartSignalChainAction, SIGNAL(triggered()), this, SLOT(startSignalChain()));

    m_pStopSignalChainAction = new QAction(QIcon(":/icons/stop.png"), tr("Stop"), this);
    connect(m_pStopSignalChainAction, SIGNAL(triggered()), this, SLOT(stopSignalChain()));

    m_pSettingsAction = new QAction(QIcon(":/icons/settings.png"), tr("Settings..."), this);
    connect(m_pSettingsAction, SIGNAL(triggered()), this, SLOT(editSettings()));
}

void MainWindow::createMenu()
{
#ifdef Q_OS_OSX
    // Qt5 has some problems with menubar on OS X
    menuBar()->setNativeMenuBar(false);
#endif

    m_pFileMenu = menuBar()->addMenu(tr("&File"));
    m_pFileMenu->addAction(m_pNewSignalChainAction);
    m_pFileMenu->addAction(m_pOpenSignalChainAction);
    m_pFileMenu->addSeparator();
    m_pFileMenu->addAction(m_pSaveSignalChainAction);
    m_pFileMenu->addAction(m_pSaveAsSignalChainAction);
    m_pFileMenu->addSeparator();
    m_pFileMenu->addAction(m_pQuitAction);

    m_pSoundMenu = menuBar()->addMenu(tr("&Sound"));
    m_pSoundMenu->addAction(m_pStartSignalChainAction);
    m_pSoundMenu->addAction(m_pStopSignalChainAction);
    m_pSoundMenu->addSeparator();
    m_pSoundMenu->addAction(m_pSettingsAction);
}

void MainWindow::createToolBars()
{
    m_pDspLoadBar = new QProgressBar();
    m_pDspLoadBar->setProperty("loadLevel", 0);
    m_pDspLoadBar->setObjectName("dspLoadBar");
    m_pDspLoadBar->setMinimum(0);
    m_pDspLoadBar->setMaximum(1000);
    m_pDspLoadBar->setMaximumWidth(200);

    m_pFileToolBar = addToolBar(tr("File"));
    m_pFileToolBar->setObjectName("fileToolBar");
    m_pFileToolBar->addAction(m_pNewSignalChainAction);
    m_pFileToolBar->addAction(m_pOpenSignalChainAction);
    m_pFileToolBar->addAction(m_pSaveSignalChainAction);

    m_pSignalChainToolBar = addToolBar(tr("Signal Chain"));
    m_pSignalChainToolBar->setObjectName("signalChainToolBar");
    m_pSignalChainToolBar->addAction(m_pStartSignalChainAction);
    m_pSignalChainToolBar->addAction(m_pStopSignalChainAction);
    QWidget *pStretch = new QWidget(this);
    pStretch->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pSignalChainToolBar->addWidget(pStretch);
    m_pSignalChainToolBar->addWidget(new QLabel(tr("DSP load ")));
    m_pSignalChainToolBar->addWidget(m_pDspLoadBar);
}

void MainWindow::updateActions()
{
    bool isStarted = m_pSignalChainWidget->scene()->signalChain()->isStarted();

    m_pNewSignalChainAction->setEnabled(!isStarted);
    m_pOpenSignalChainAction->setEnabled(!isStarted);
    m_pSaveSignalChainAction->setEnabled(!isStarted);
    m_pSaveAsSignalChainAction->setEnabled(!isStarted);

    m_pStartSignalChainAction->setVisible(!isStarted);
    m_pStopSignalChainAction->setVisible(isStarted);
    m_pSettingsAction->setEnabled(!isStarted);
}

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
}

void MainWindow::loadSettings()
{
    QSettings settings;
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
}
