#ifndef LOGGER_H
#define LOGGER_H

#include <QObject.h>
#include "FrameworkApi.h"

class QMUSIC_FRAMEWORK_API Logger : public QObject
{
    Q_OBJECT
public:

    enum Level {
        Level_Debug,
        Level_Info,
        Level_Warning,
        Level_Error
    };

    Logger(QObject *pParent = nullptr);

    static QString levelToString(Level level);

public slots:

    void log(Level level, const QString &text);

signals:

    void logged(Logger::Level level, const QString &text);
    void logged0(int level, const QString &text);

private slots:

    void onLogged0(int level, const QString &text);

};

Q_DECLARE_METATYPE(Logger::Level)

#endif // LOGGER_H

