#include <QDebug>
#include <QIcon>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include "Settings.h"
#include "AudioDevice.h"
#include "MidiInputDevice.h"
#include "../include/SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *pParent)
    : QDialog(pParent)
{
    setWindowTitle(tr("Settings"));
    setWindowIcon(QIcon(":/icons/settings.png"));

    createLayout();
    enumerateDevices();

    // Fix dialog box size.
    layout()->setSizeConstraint(QLayout::SetFixedSize);
    setModal(true);
}

void SettingsDialog::loadSettings()
{
    Settings settings;

    int index = m_pWaveInComboBox->findData(settings.get(Settings::Setting_WaveInIndex).toInt());
    if (index >= 0) {
        m_pWaveInComboBox->setCurrentIndex(index);
    }

    index = m_pWaveOutComboBox->findData(settings.get(Settings::Setting_WaveOutIndex).toInt());
    if (index >= 0) {
        m_pWaveOutComboBox->setCurrentIndex(index);
    } else {
        qDebug() << "Unable to find output index" << index;
    }

    bool ok = false;
    double sampleRate = settings.get(Settings::Setting_SampleRate).toDouble(&ok);
    if (ok) {
        m_pSampleRateComboBox->setEditText(QString::number(sampleRate));
    }

    int bufferSize = settings.get(Settings::Setting_BufferSize).toInt(&ok);
    if (ok) {
        m_pBufferSizeSpinBox->setValue(bufferSize);
    }

    index = m_pMidiInComboBox->findData(settings.get(Settings::Setting_MidiInIndex).toInt());
    if (index >= 0) {
        m_pMidiInComboBox->setCurrentIndex(index);
    }

    m_pMidiInChannelSpinBox->setValue(settings.get(Settings::Setting_MidiInChannel).toInt());
}

void SettingsDialog::saveSettings()
{
    Settings settings;
    settings.set(Settings::Setting_WaveInIndex, m_pWaveInComboBox->currentData().toInt());
    settings.set(Settings::Setting_WaveOutIndex, m_pWaveOutComboBox->currentData().toInt());
    bool ok = false;
    double sampleRate = m_pSampleRateComboBox->currentText().toDouble(&ok);
    if (ok) {
        settings.set(Settings::Setting_SampleRate, sampleRate);
    }
    settings.set(Settings::Setting_BufferSize, m_pBufferSizeSpinBox->value());

    settings.set(Settings::Setting_MidiInIndex, m_pMidiInComboBox->currentData().toInt());
    settings.set(Settings::Setting_MidiInChannel, m_pMidiInChannelSpinBox->value());
}

void SettingsDialog::createLayout()
{
    // Audio devices
    QFormLayout *pFormLayout = new QFormLayout();
    m_pWaveInComboBox = new QComboBox();
    m_pWaveOutComboBox = new QComboBox();
    m_pSampleRateComboBox = new QComboBox();
    m_pSampleRateComboBox->setEditable(true);
    m_pSampleRateComboBox->setInsertPolicy(QComboBox::NoInsert);
    m_pBufferSizeSpinBox = new QSpinBox();
    m_pBufferSizeSpinBox->setMinimum(16);
    m_pBufferSizeSpinBox->setMaximum(16 * 1024);
    m_pMidiInComboBox = new QComboBox();
    m_pMidiInChannelSpinBox = new QSpinBox();
    m_pMidiInChannelSpinBox->setMinimum(1);
    m_pMidiInChannelSpinBox->setMaximum(16);

    pFormLayout->addRow(tr("Wave In"), m_pWaveInComboBox);
    pFormLayout->addRow(tr("Wave Out"), m_pWaveOutComboBox);
    pFormLayout->addRow(tr("Sample rate"), m_pSampleRateComboBox);
    pFormLayout->addRow(tr("Buffer size"), m_pBufferSizeSpinBox);
    pFormLayout->addRow(new QLabel());
    pFormLayout->addRow(tr("MIDI In"), m_pMidiInComboBox);
    pFormLayout->addRow(tr("MIDI In channel"), m_pMidiInChannelSpinBox);

    // Create buttons
    QPushButton *pOkButton = new QPushButton(tr("OK"));
    pOkButton->setDefault(true);
    connect(pOkButton, SIGNAL(clicked()), this, SLOT(accept()));
    QPushButton *pCancelButton = new QPushButton(tr("Cancel"));
    connect(pCancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    QHBoxLayout *pButtonsLayout = new QHBoxLayout();
    pButtonsLayout->addStretch();
    pButtonsLayout->addWidget(pOkButton);
    pButtonsLayout->addWidget(pCancelButton);

    QVBoxLayout *pTopLayout = new QVBoxLayout();
    pTopLayout->addLayout(pFormLayout);
    pTopLayout->addLayout(pButtonsLayout);

    setLayout(pTopLayout);
}

void SettingsDialog::enumerateDevices()
{
    AudioDevice audioDevice;
    QList<AudioDevice::Info> list = audioDevice.enumarate();

    QList<double> sampleRates;
    sampleRates.append(44100.0);    // Make sure we have at least the default sample rate.

    foreach (const AudioDevice::Info info, list) {
        if (info.nInputs > 0) {
            // Input device detected
            m_pWaveInComboBox->addItem(info.name, info.index);
        }
        if (info.nOutputs > 0) {
            // Output device detected
            m_pWaveOutComboBox->addItem(info.name, info.index);
        }

        double sampleRate = info.sampleRate;
        if (!sampleRates.contains(sampleRate)) {
            sampleRates.append(sampleRate);
        }
    }

    foreach (double rate, sampleRates) {
        m_pSampleRateComboBox->addItem(QString::number(rate), QVariant(rate));
    }

    // Enumerate MIDI devices
    QList<MidiDevice::Description> midiDevices = MidiDevice::enumerateInputDevices();
    foreach (const MidiDevice::Description desc, midiDevices) {
        if (desc.type == MidiDevice::Type_Input) {
            m_pMidiInComboBox->addItem(desc.name, desc.number);
        }
    }
}
