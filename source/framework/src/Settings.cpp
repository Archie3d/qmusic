#include <QMap>
#include <QSettings>
#include "Application.h"
#include "Settings.h"

// Settings default values
const QMap<Settings::Setting, QVariant> cDefaultSettings = []() {
    QMap<Settings::Setting, QVariant> map;
    map[Settings::Setting_WaveInIndex] = 0;
    map[Settings::Setting_MidiOutIndex] = 0;
    map[Settings::Setting_MidiInIndex] = 0;
    map[Settings::Setting_MidiOutIndex] = 0;
    map[Settings::Setting_SampleRate] = 44100.0;
    return map;
}();

const QMap<Settings::Setting, QString> cSettingsNameMap = []() {
    QMap<Settings::Setting, QString> map;
    map[Settings::Setting_WaveInIndex] = "waveInIndex";
    map[Settings::Setting_MidiOutIndex] = "waveOutIndex";
    map[Settings::Setting_MidiInIndex] = "midiInIndex";
    map[Settings::Setting_MidiOutIndex] = "midiOutIndex";
    map[Settings::Setting_SampleRate] = "sampleRate";
    return map;
}();

Settings::Settings()
    : m_settings(Application::Company, Application::Product)
{
}

QVariant Settings::get(Settings::Setting s) const
{
    return m_settings.value(cSettingsNameMap[s], cDefaultSettings[s]);
}

void Settings::set(Settings::Setting s, const QVariant &value)
{
    m_settings.setValue(cSettingsNameMap[s], value);
}
