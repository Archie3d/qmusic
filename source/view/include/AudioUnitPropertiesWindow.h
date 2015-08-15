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

#ifndef AUDIOUNITPROPERTIESWINDOW_H
#define AUDIOUNITPROPERTIESWINDOW_H

#include <QDockWidget>
#include "ViewApi.h"

class QtTreePropertyBrowser;
class QtVariantEditorFactory;
class AudioUnit;

/**
 * Window showing the list of properties of currently selected audio unit.
 */
class QMUSIC_VIEW_API AudioUnitPropertiesWindow : public QDockWidget
{
    Q_OBJECT
public:

    AudioUnitPropertiesWindow(QWidget *pParent = nullptr);

    /**
     * Display (and allow editing) properties of the audio unit.
     * @param pAudioUnit
     */
    void setAudioUnit(AudioUnit *pAudioUnit);

    /**
     * Clear the properties browser.
     */
    void clear();

public slots:

    void handleAudioUnitSelected(AudioUnit *pAudioUnit);

private:
    QtTreePropertyBrowser *m_pPropertyBrowser;
    QtVariantEditorFactory *m_pPropertyFactory;
};

#endif // AUDIOUNITPROPERTIESWINDOW_H

