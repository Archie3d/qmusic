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

#include <QHash>
#include "Logger.h"

const QHash<Logger::Level, QString> cLogLevelToStringMap = []() {
    QHash<Logger::Level, QString> map;
    map[Logger::Level_Debug] = QObject::tr("Debug");
    map[Logger::Level_Info] = QObject::tr("Info");
    map[Logger::Level_Warning] = QObject::tr("Warning");
    map[Logger::Level_Error] = QObject::tr("Error");
    return map;
}();

Logger::Logger(QObject *pParent)
    : QObject(pParent)
{
    connect(this, SIGNAL(logged0(int,QString)),
            this, SLOT(onLogged0(int,QString)), Qt::QueuedConnection);
}

QString Logger::levelToString(Level level)
{
    return cLogLevelToStringMap.value(level);
}

void Logger::log(Level level, const QString &text)
{
    emit logged0(static_cast<int>(level), text);
}

void Logger::onLogged0(int level, const QString &text)
{
    emit logged(static_cast<Level>(level), text);
}
