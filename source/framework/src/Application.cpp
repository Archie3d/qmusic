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

#ifdef _MSC_VER
#   include <Windows.h>
#   include <ShlObj.h>
#   include <NTSecAPI.h>
#endif // _MSC_VER

#include <QDebug>
#include <QTimer>
#include <QClipboard>
#include <QMimeData>
#include <QFile>
#include <QStandardPaths>
#include <QStyleFactory>
#include "Logger.h"
#include "AudioDevicesManager.h"
#include "AudioUnitsManager.h"
#include "EventRouter.h"
#include "CrashReporter.h"
#include "Application.h"

const QString Application::Company("Archie3d");
const QString Application::Domain("archie3d.net");
const QString Application::Product("QMusic");

// Application instamce
static Application* s_pApplicationInstance = nullptr;

static void msgHandler(QtMsgType type, const QMessageLogContext &contect, const QString &text)
{
    switch (type) {
    case QtFatalMsg:
    case QtCriticalMsg:
        logError(text);
        break;
    case QtWarningMsg:
        logWarning(text);
        break;
    case QtDebugMsg:
    default:
        logDebug0(text);
        break;
    }
}

Application* Application::instance()
{
    return s_pApplicationInstance;
}

Application::Application(int &argc, char **argv)
    : QApplication(argc, argv),
      m_pMainWindow(nullptr)
{
    Q_ASSERT(s_pApplicationInstance == nullptr);

    setApplicationName(Product);
    setApplicationVersion(QMUSIC_VERSION);

    // Install crash reporter
    CrashReporter *pCR = CrashReporter::instance();
    Q_ASSERT(pCR != nullptr);

    setOrganizationName(Company);
    setOrganizationDomain(Domain);
    setApplicationName(Product);

    m_pLogger = new Logger(this);
    m_pAudioDevicesManager = new AudioDevicesManager(this);
    m_pAudioUnitsManager = new AudioUnitsManager(this);
    m_pEventRouter = new EventRouter(this);

    loadStylesheet();

    s_pApplicationInstance = this;
}

Application::~Application()
{
    if (m_pAudioUnitsManager != nullptr) {
        m_pAudioUnitsManager->cleanup();
    }

    s_pApplicationInstance = nullptr;
}

void Application::setMainWindow(QMainWindow *pMainWindow)
{
    Q_ASSERT(pMainWindow != nullptr);
    Q_ASSERT(m_pMainWindow == nullptr);

    m_pMainWindow = pMainWindow;
}

QByteArray Application::clipboardData(const QString &mimeId)
{
    const QClipboard *pClipboard = QApplication::clipboard();
    const QMimeData *pMimeData = pClipboard->mimeData();

    for (QString format : pMimeData->formats()) {
        QByteArray data = pMimeData->data(format);
        if (format.startsWith("application/x-qt")) {
            // Retrieving true format name
            int indexBegin = format.indexOf('"') + 1;
            int indexEnd = format.indexOf('"', indexBegin);
            format = format.mid(indexBegin, indexEnd - indexBegin);
        }

        if (format == mimeId) {
            return data;
        }
    }

    return QByteArray();
}

QDir Application::dataDirectory()
{
    QString path = QDir::homePath();
#ifdef _MSC_VER
    TCHAR szPath[MAX_PATH];
    BOOL res = SHGetSpecialFolderPath(0, szPath, CSIDL_APPDATA, 1 /* Create if does not exist */);
    if (res == TRUE) {
        path = QString::fromLatin1(szPath);
    }
    path += "\\" + applicationName();
    QDir dir(path);
    if (!dir.exists()) {
        QDir().mkpath(dir.absolutePath());
    }
    return QDir(path);
#endif // _MSC_VER
}

QDir Application::documentsDirectory()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    return QDir(path);
}

int Application::launch()
{
    // Install custom message handler for Qt logging
    qInstallMessageHandler(msgHandler);

    if (m_pMainWindow != nullptr) {
        m_pMainWindow->setVisible(true);
    }

    QTimer::singleShot(0, this, &Application::initialize);

    return exec();
}

void Application::loadStylesheet()
{
    setStyle(QStyleFactory::create("fusion"));

    QPalette palette;
    palette.setColor(QPalette::Window, QColor(53,53,53));
    palette.setColor(QPalette::WindowText, QColor(120, 115, 110)); // 160, 155, 150
    palette.setColor(QPalette::Base, QColor(15,15,15));
    palette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    palette.setColor(QPalette::ToolTipBase, Qt::white);
    palette.setColor(QPalette::ToolTipText, Qt::white);
    palette.setColor(QPalette::Text, QColor(180, 170, 165));
    palette.setColor(QPalette::Button, QColor(53,53,53));
    palette.setColor(QPalette::ButtonText, QColor(180, 180, 180));
    palette.setColor(QPalette::BrightText, Qt::red);

    palette.setColor(QPalette::Highlight, QColor(142,45,197).lighter());
    palette.setColor(QPalette::HighlightedText, Qt::black);

    palette.setColor(QPalette::Disabled, QPalette::Text, Qt::darkGray);
    palette.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::darkGray);
    setPalette(palette);


    QFile f(":/framework/style.qss");
    if (f.open(QIODevice::ReadOnly)) {
        QString styleSheet = QString::fromUtf8(f.readAll());
        setStyleSheet(styleSheet);
        f.close();
    }
}


void Application::initialize()
{
    Q_ASSERT(m_pAudioUnitsManager != nullptr);

    // Load audio units
    m_pAudioUnitsManager->initialize();
}

void logDebug0(const QString &msg)
{
    if (Application::instance() != nullptr) {
        Application::instance()->logger()->log(Logger::Level_Debug, msg);
    }
}

void logInfo(const QString &msg)
{
    if (Application::instance() != nullptr) {
        Application::instance()->logger()->log(Logger::Level_Info, msg);
    }
}

void logWarning(const QString &msg)
{
    if (Application::instance() != nullptr) {
        Application::instance()->logger()->log(Logger::Level_Warning, msg);
    }
}

void logError(const QString &msg)
{
    if (Application::instance() != nullptr) {
        Application::instance()->logger()->log(Logger::Level_Error, msg);
    }
}
