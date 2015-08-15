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

#include <QApplication>
#include <QMainWindow>
#include "FrameworkApi.h"

class Logger;
class AudioDevice;
class MidiInputDevice;
class AudioUnitsManager;

/**
 * The application.
 *
 * This class is a singleton of the global aplpication object.
 *
 * The application holds other global objects, line audio devices (input and output),
 * MIDI devices (input) and audio units manager. It also references the main window
 * of the GUI.
 */
class QMUSIC_FRAMEWORK_API Application : public QApplication
{
    Q_OBJECT
public:

    /// Company string.
    const static QString Company;

    /// Company domain.
    const static QString Domain;

    /// Product name.
    const static QString Product;

    /**
     * Returns the application instance.
     * @return
     */
    static Application* instance();

    Application(int argc, char **argv);
    ~Application();

    Logger* logger() const { return m_pLogger; }
    AudioDevice* audioInputDevice() const { return m_pAudioInputDevice; }
    AudioDevice* audioOutputDevice() const {return m_pAudioOutputDevice; }
    MidiInputDevice* midiInputDevice() const { return m_pMidiInputDevice; }
    AudioUnitsManager* audioUnitsManager() const { return m_pAudioUnitsManager; }

    void setMainWindow(QMainWindow *pMainWindow);
    QMainWindow* mainWindow() const { return m_pMainWindow; }

    /**
     * Returns data from the clipboard by its mime id.
     * @param mimeId
     * @return
     */
    QByteArray clipboardData(const QString &mimeId);

public slots:

    /**
     * Launch the application
     * @return Exit code.
     */
    int launch();

private slots:

    /**
     * Load and apply global application stylesheet.
     */
    void loadStylesheet();

    /**
     * @brief Initialize application.
     */
    void initialize();

private:

    /// Application main window.
    QMainWindow *m_pMainWindow;

    /// Logger.
    Logger *m_pLogger;

    /// Pointer to audio input device.
    AudioDevice *m_pAudioInputDevice;

    /// Pointer to audio output device;
    AudioDevice *m_pAudioOutputDevice;

    /// Pointer to MIDI input device;
    MidiInputDevice *m_pMidiInputDevice;

    /// Pointer to audio units manager (single instance per application).
    AudioUnitsManager *m_pAudioUnitsManager;
};

QMUSIC_FRAMEWORK_API void logDebug0(const QString &text);
QMUSIC_FRAMEWORK_API void logInfo(const QString &text);
QMUSIC_FRAMEWORK_API void logWarning(const QString &text);
QMUSIC_FRAMEWORK_API void logError(const QString &text);

// Debug messages are logged with line numbers
#ifdef Q_OS_WIN
#   define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#   define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#define logDebug(msg)   logDebug0(QString("<b><u>%1:%2</u></b> ").arg(__FILENAME__).arg(__LINE__) + (msg))
