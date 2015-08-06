#include <QDebug>
#include <QTimer>
#include "Logger.h"
#include "AudioDevice.h"
#include "AudioUnitsManager.h"
#include "Application.h"

const QString Application::Company("Archie3d");
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

Application::Application(int argc, char **argv)
    : QApplication(argc, argv),
      m_pMainWindow(nullptr)
{
    Q_ASSERT(s_pApplicationInstance == nullptr);

    m_pLogger = new Logger(this);
    m_pAudioInputDevice = new AudioDevice();
    m_pAudioOutputDevice = new AudioDevice();
    m_pAudioUnitsManager = new AudioUnitsManager(this);

    s_pApplicationInstance = this;
}

Application::~Application()
{
    if (m_pAudioUnitsManager != nullptr) {
        m_pAudioUnitsManager->cleanup();
    }

    delete m_pAudioInputDevice;
    delete m_pAudioOutputDevice;

    s_pApplicationInstance = nullptr;
}

void Application::setMainWindow(QMainWindow *pMainWindow)
{
    Q_ASSERT(pMainWindow != nullptr);
    Q_ASSERT(m_pMainWindow == nullptr);

    m_pMainWindow = pMainWindow;
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
