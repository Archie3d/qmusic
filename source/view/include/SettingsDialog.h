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
};

#endif // SETTINGSDIALOG_H

