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

#include <QToolBox>
#include "Application.h"
#include "AudioUnitsManager.h"
#include "AudioUnitsCategoryWidget.h"
#include "AudioUnitsManagerWindow.h"

AudioUnitsManagerWindow::AudioUnitsManagerWindow(QWidget *pParent)
    : QDockWidget(tr("Audio Units"), pParent)
{
    setObjectName("AudioUnitsManagerWindow");

    m_pToolBox = new QToolBox();
    setWidget(m_pToolBox);

    connect(Application::instance()->audioUnitsManager(), SIGNAL(initialized()),
            this, SLOT(populate()));
}

void AudioUnitsManagerWindow::populate()
{
    QStringList categories = Application::instance()->audioUnitsManager()->categories();
    foreach (const QString &category, categories) {
        AudioUnitsCategoryWidget *pCategoryWidget = new AudioUnitsCategoryWidget(category);
        m_pToolBox->addItem(pCategoryWidget, category);
    }
}
