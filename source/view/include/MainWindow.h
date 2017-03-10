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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ViewApi.h"

class QAction;
class QProgressBar;
class LogWindow;
class AudioUnitsManagerWindow;
class AudioUnitPropertiesWindow;
class SpectrumWindow;
class PianoKeyboardWindow;
class SignalChainWidget;

/**
 * Application main window.
 */
class QMUSIC_VIEW_API MainWindow : public QMainWindow
{
    Q_OBJECT
public:

    MainWindow(QWidget *pParent = nullptr, Qt::WindowFlags flags = 0);
    ~MainWindow();

    LogWindow* logWindow() const { return m_pLogWindow; }
    AudioUnitsManagerWindow* audioUnitsManagerWindow() const { return m_pAudioUnitsManagerWindow; }
    AudioUnitPropertiesWindow* audioUnitPropertiesWindow() const { return m_pAudioUnitPropertiesWindow; }
    SpectrumWindow* spectrumWindow() const { return m_pSpectrumWindow; }
    PianoKeyboardWindow* pianoKeyboardWindow() const { return m_pPianoKeyboardWindow; }

public slots:

    /**
     * Update the load bar with a new value.
     * @param l Load, [0..1].
     */
    void updateDspLoad(float l);

protected:

    void closeEvent(QCloseEvent *pEvent);

private slots:

    void newSignalChain();
    void saveSignalChain();
    void saveAsSignalChain();
    void openSignalChain();

    void startSignalChain();
    void stopSignalChain();
    void editSettings();

private:

    void createDockingWindows();
    void createActions();
    void createMenu();
    void createToolBars();
    void updateActions();
    void saveSettings();
    void loadSettings();

    LogWindow *m_pLogWindow;    
    AudioUnitsManagerWindow *m_pAudioUnitsManagerWindow;
    AudioUnitPropertiesWindow *m_pAudioUnitPropertiesWindow;
    SpectrumWindow *m_pSpectrumWindow;
    PianoKeyboardWindow *m_pPianoKeyboardWindow;

    /// Central widget showing a signal chain.
    SignalChainWidget *m_pSignalChainWidget;

    // Actions
    QAction *m_pNewSignalChainAction;
    QAction *m_pOpenSignalChainAction;
    QAction *m_pSaveSignalChainAction;
    QAction *m_pSaveAsSignalChainAction;
    QAction *m_pQuitAction;

    QAction *m_pStartSignalChainAction;
    QAction *m_pStopSignalChainAction;
    QAction *m_pSettingsAction;

    // Menus
    QMenuBar *m_pMenuBar;
    QMenu *m_pFileMenu;
    QMenu *m_pSoundMenu;

    // Toolbars
    QToolBar *m_pFileToolBar;
    QToolBar *m_pSignalChainToolBar;

    // Load bar
    QProgressBar *m_pDspLoadBar;
};

#endif // MAINWINDOW_H

