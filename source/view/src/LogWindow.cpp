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
#include <QDateTime>
#include <QTextEdit>
#include <QListView>
#include "Application.h"
#include "LogWindow.h"

QHash<Logger::Level, QColor> cLogLevelColorMap = {
    {Logger::Level_Debug, QColor("grey")},
    {Logger::Level_Info, QColor("black")},
    {Logger::Level_Warning, QColor("orange")},
    {Logger::Level_Error, QColor("red")}
};

LogWindow::LogWindow(QWidget *pParent)
    : QDockWidget(tr("Console"), pParent)
{
    setObjectName("LogWindow");

    m_pLogText = new QTextEdit(this);
    m_pLogText->setReadOnly(true);
    m_pLogText->setFont(QFont("Consolas", 9));
    m_pLogText->setFrameStyle(QFrame::NoFrame);

    setWidget(m_pLogText);

    QListView;

    connect(Application::instance()->logger(), SIGNAL(logged(Logger::Level,QString)),
            this, SLOT(onLogged(Logger::Level,QString)));
}

void LogWindow::clear()
{
    m_pLogText->clear();
}

void LogWindow::onLogged(Logger::Level level, const QString &text)
{
#ifdef NDEBUG
    // Hide debug messages in release build
    if (Logger::Level_Debug == level) {
        return;
    }
#endif

    QColor color = cLogLevelColorMap.value(level, QColor("black"));
    QString strDateTime = QDateTime::currentDateTime().toString();

    m_pLogText->moveCursor(QTextCursor::End);
    m_pLogText->setTextColor(QColor("navy"));
    m_pLogText->insertPlainText(strDateTime);
    m_pLogText->moveCursor(QTextCursor::End);
    m_pLogText->setTextColor(color);

    QString strLevel = Logger::levelToString(level);
    strLevel += QString(7 - strLevel.length(),  ' ');   // Align log level vs message text
    QString line = QString(" %1 ").arg(strLevel);
    m_pLogText->insertPlainText(line);
    m_pLogText->moveCursor(QTextCursor::End);
    m_pLogText->insertHtml(text);
    m_pLogText->moveCursor(QTextCursor::End);
    m_pLogText->setTextColor(QColor("black"));
    m_pLogText->insertPlainText("\n");
    m_pLogText->moveCursor(QTextCursor::End);
}
