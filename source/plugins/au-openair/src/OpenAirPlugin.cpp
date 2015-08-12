#include <QDebug>
#include "WavFile.h"
#include "OpenAirPlugin.h"
#include "OpenAir.h"

OpenAirPlugin::OpenAirPlugin(QObject *pParent)
    : AudioUnitPlugin(pParent)
{
}

QIcon OpenAirPlugin::icon() const
{
    return QIcon(":/au-openair/icon.png");
}

void OpenAirPlugin::initialize()
{
    m_irToFile.clear();
    m_irToFile["Cathedral"] = ":/au-openair/ir/ir2_dc.wav";
    m_irToFile["The Lady Chapel, St Albans Cathedral"] = ":/au-openair/ir/stalbans_omni.wav";
    m_irToFile["Underground Car Park"] = ":/au-openair/ir/carpark_balloon_ir_mono_24bit_44100.wav";
}

AudioUnit* OpenAirPlugin::createInstance()
{
    return new OpenAir(this);
}

QStringList OpenAirPlugin::environments() const
{
    return m_irToFile.keys();
}

QVector<float> OpenAirPlugin::impulseResponse(const QString &env) const
{
    QVector<float> defaultResponse;
    defaultResponse.append(1.0f);

    if (!m_irToFile.contains(env)) {
        return defaultResponse;
    }

    WavFile wf(m_irToFile.value(env));
    if (!wf.open()) {
        qCritical() << "Unable to find IR data";
        return defaultResponse;
    }

    if (!wf.readHeader()) {
        qCritical() << "Unable to parse IR data header";
        return defaultResponse;
    }

    if (wf.format() != WavFile::Format_PCM || wf.numberOfChannels() != 1) {
        qCritical() << "IR data format is invalid";
        qDebug() << "Format:" << wf.format() << wf.numberOfChannels();
        return defaultResponse;
    }

    QVector<float> response;
    if (!wf.readSingleChannelData(response)) {
        qCritical() << "Unable to read IR coefficients";
        return defaultResponse;
    }

    return response;
}
