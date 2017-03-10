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

#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include "FrameworkApi.h"

/**
 * @brief This object is used to log application events (log book).
 */
class QMUSIC_FRAMEWORK_API Logger : public QObject
{
    Q_OBJECT
public:

    /// Logging level.
    enum Level {
        Level_Debug,    ///< Debug information (not visible in release builds).
        Level_Info,     ///< Informative message.
        Level_Warning,  ///< Warning message.
        Level_Error     ///< Error message.
    };

    /**
     * Construct a logger.
     * @param pParent
     */
    Logger(QObject *pParent = nullptr);

    /**
     * Convert log level to its textual representation.
     * @param level
     * @return
     */
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

