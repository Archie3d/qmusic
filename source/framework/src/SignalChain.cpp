#include <QThread>
#include "Application.h"
#include "Settings.h"
#include "AudioDevice.h"
#include "AudioUnit.h"
#include "SerializationContext.h"
#include "SignalChain.h"

const QString SignalChain::UID("SignalChain");

SignalChain::SignalChain()
    : m_timeStep(0.0),
      m_started(false),
      m_audioUnits()
{
}

SignalChain::~SignalChain()
{
    qDeleteAll(m_audioUnits);
}

void SignalChain::start()
{
    if (isStarted()) {
        return;
    }
    startAudioDevices();
    startAllAudioUnits();
    m_started = true;
}

void SignalChain::stop()
{
    if (!isStarted()) {
        return;
    }

    stopAllAudioUnits();
    stopAudioDevices();
    m_started = false;
}

void SignalChain::reset()
{
    if (isStarted()) {
        // Cannot reset when running.
        return;
    }
    resetAllAudioUnits();
}

void SignalChain::addAudioUnit(IAudioUnit *pAudioUnit)
{
    Q_ASSERT(pAudioUnit != nullptr);
    if (m_audioUnits.contains(pAudioUnit)) {
        return;
    }

    AudioUnit *pAU = dynamic_cast<AudioUnit*>(pAudioUnit);
    if (pAU) {
        pAU->setSignalChain(this);
    }
    m_audioUnits.append(pAudioUnit);
}

void SignalChain::removeAudioUnit(IAudioUnit *pAudioUnit)
{
    Q_ASSERT(pAudioUnit != nullptr);
    AudioUnit *pAU = dynamic_cast<AudioUnit*>(pAudioUnit);
    if (pAU) {
        pAU->setSignalChain(nullptr);
    }
    m_audioUnits.removeOne(pAudioUnit);
}

void SignalChain::prepareUpdate()
{
    foreach (IAudioUnit *pAudioUnit, m_audioUnits) {
        pAudioUnit->prepareUpdate();
    }
}


void SignalChain::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);

    QVariantList units;
    foreach (IAudioUnit *pAu, m_audioUnits) {
        units.append(pContext->serialize(pAu));
    }
    data["audioUnits"] = units;
}

void SignalChain::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);

    // Delete all existing audio units
    qDeleteAll(m_audioUnits);

    QVariantList units = data["audioUnits"].toList();
    foreach (const QVariant &v, units) {
        ISerializable *pSer = pContext->deserialize(v);
        IAudioUnit *pAu = dynamic_cast<IAudioUnit*>(pSer);
        addAudioUnit(pAu);
    }
}

void SignalChain::startAudioDevices()
{
    Settings settings;

    AudioDevice* pInDev = Application::instance()->audioInputDevice();
    AudioDevice* pOutDev = Application::instance()->audioOutputDevice();

    int waveInDeviceIndex = settings.get(Settings::Setting_WaveInIndex).toInt();
    int waveOutDeviceIndex = settings.get(Settings::Setting_WaveOutIndex).toInt();
    double sampleRate = settings.get(Settings::Setting_SampleRate).toDouble();
    int bufferSize = settings.get(Settings::Setting_BufferSize).toInt();

    setTimeStep(1.0 / sampleRate);

    if (pInDev != nullptr) {
        if (pInDev->open(waveInDeviceIndex, 2, 0, sampleRate, bufferSize)) {
            pInDev->start();
        }
    }

    if (pOutDev != nullptr) {
        if (pOutDev->open(waveOutDeviceIndex, 0, 2, sampleRate, bufferSize)) {
            pOutDev->start();
        }
    }
}

void SignalChain::stopAudioDevices()
{
    AudioDevice *pInDev = Application::instance()->audioInputDevice();
    AudioDevice *pOutDev = Application::instance()->audioOutputDevice();

    if (pInDev != nullptr) {
        pInDev->stop();
        pInDev->close();
    }

    if (pOutDev != nullptr) {
        pOutDev->stop();
        pOutDev->close();
    }
}

void SignalChain::startAllAudioUnits()
{
    foreach (IAudioUnit *pAudioUnit, m_audioUnits) {
        pAudioUnit->start();
    }
}

void SignalChain::stopAllAudioUnits()
{
    foreach (IAudioUnit *pAudioUnit, m_audioUnits) {
        pAudioUnit->stop();
    }
}

void SignalChain::resetAllAudioUnits()
{
    foreach (IAudioUnit *pAudioUnit, m_audioUnits) {
        pAudioUnit->reset();
    }
}
