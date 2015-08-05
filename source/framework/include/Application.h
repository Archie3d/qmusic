#include <QApplication>
#include <QMainWindow>
#include "FrameworkApi.h"

class Logger;
class AudioDevice;
class AudioUnitsManager;

class QMUSIC_FRAMEWORK_API Application : public QApplication
{
    Q_OBJECT
public:

    const static QString Company;
    const static QString Product;

    static Application* instance();

    Application(int argc, char **argv);
    ~Application();

    Logger* logger() const { return m_pLogger; }
    AudioDevice* audioDevice() const { return m_pAudioDevice; }
    AudioUnitsManager* audioUnitsManager() const { return m_pAudioUnitsManager; }

    void setMainWindow(QMainWindow *pMainWindow);
    QMainWindow* mainWindow() const { return m_pMainWindow; }

public slots:

    int launch();

private slots:

    /**
     * @brief Initialize application.
     */
    void initialize();

private:

    /// Application main window.
    QMainWindow *m_pMainWindow;

    /// Logger.
    Logger *m_pLogger;

    /// Pointer to audio device.
    AudioDevice *m_pAudioDevice;

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
