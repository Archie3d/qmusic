#ifndef LOGGER_H
#define LOGGER_H

#include <QObject.h>
#include "FrameworkApi.h"

/**
 * This object is used to log application events (log book).
 */
class QMUSIC_FRAMEWORK_API Logger : public QObject
{
    Q_OBJECT
public:

    /// Logging level.
    enum Level {
        Level_Debug,
        Level_Info,
        Level_Warning,
        Level_Error
    };

    Logger(QObject *pParent = nullptr);

    static QString levelToString(Level level);

public slots:

    /**
     * Log an event.
     * @param level Log level.
     * @param text Text to be logged.
     */
    void log(Level level, const QString &text);

signals:

    /**
     * Signal to notify the logged event.
     * This signal is emitted via queued connection in order to
     * prevent infinite loop in case when the signal handler will
     * log something in its turn.
     * @param level
     * @param text
     */
    void logged(Logger::Level level, const QString &text);
    void logged0(int level, const QString &text);

private slots:

    void onLogged0(int level, const QString &text);

};

Q_DECLARE_METATYPE(Logger::Level)

#endif // LOGGER_H

