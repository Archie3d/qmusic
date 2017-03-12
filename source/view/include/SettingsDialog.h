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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "ViewApi.h"

class QComboBox;
class QSpinBox;

/**
 * @brief Dialog box to present and edit global settings.
 */
class QMUSIC_VIEW_API SettingsDialog : public QDialog
{
    Q_OBJECT
public:

    SettingsDialog(QWidget *pParent = nullptr);

    /**
     * Populate dialog with current settings values.
     */
    void loadSettings();

    /**
     * Dump dialog controls to global settings object.
     */
    void saveSettings();

private:

    /// Create dialog layout.
    void createLayout();

    void enumerateDevices();

    QComboBox *m_pWaveInComboBox;
    QComboBox *m_pWaveOutComboBox;
    QComboBox *m_pSampleRateComboBox;
    QSpinBox *m_pBufferSizeSpinBox;

    QComboBox *m_pMidiInComboBox;
    QComboBox *m_pMidiInChannelComboBox;
};

#endif // SETTINGSDIALOG_H

