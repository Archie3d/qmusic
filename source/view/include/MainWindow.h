#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ViewApi.h"

class QAction;
class LogWindow;
class AudioUnitsManagerWindow;
class AudioUnitPropertiesWindow;
class SignalChainWidget;

class QMUSIC_VIEW_API MainWindow : public QMainWindow
{
    Q_OBJECT
public:

    MainWindow(QWidget *pParent = nullptr, Qt::WindowFlags flags = 0);
    ~MainWindow();

private slots:

    void startSignalChain();
    void stopSignalChain();

private:

    void createDockingWindows();
    void createActions();
    void createToolBars();

    void updateActions();

    LogWindow *m_pLogWindow;    
    AudioUnitsManagerWindow *m_pAudioUnitsManagerWindow;
    AudioUnitPropertiesWindow *m_pAudioUnitPropertiesWindow;

    /// Central widget showing a signal chain.
    SignalChainWidget *m_pSignalChainWidget;

    // Actions
    QAction *m_pStartSignalChainAction;
    QAction *m_pStopSignalChainAction;

    // Toolbars
    QToolBar *m_pSignalChainToolBar;
};

#endif // MAINWINDOW_H

