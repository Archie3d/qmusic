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

#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QDockWidget>
#include "ViewApi.h"
#include "Logger.h"

class QTextEdit;

/**
 * Logging window.
 * This object captured log events from application logger and display then in a text box.
 * @see Logger
 */
class QMUSIC_VIEW_API LogWindow : public QDockWidget
{
    Q_OBJECT

public:

    LogWindow(QWidget *pParent = nullptr);

public slots:

    /**
     * @brief Clear log window.
     */
    void clear();

private slots:

    void onLogged(Logger::Level level, const QString &text);

private:

    QTextEdit *m_pLogText;
};

#endif // LOGWINDOW_H

