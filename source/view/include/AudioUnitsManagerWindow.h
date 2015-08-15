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

#ifndef AUDIOUNITSMANAGERWINDOW_H
#define AUDIOUNITSMANAGERWINDOW_H

#include <QDockWidget>
#include "ViewApi.h"

class QToolBox;

/**
 * This window shows the categorized list of all available audio units.
 */
class QMUSIC_VIEW_API AudioUnitsManagerWindow : public QDockWidget
{
    Q_OBJECT
public:

    AudioUnitsManagerWindow(QWidget *pParent = nullptr);

public slots:

    /**
     * Populate the view with the audio units.
     */
    void populate();

private:

    QToolBox *m_pToolBox;
};

#endif // AUDIOUNITSMANAGERWINDOW_H

