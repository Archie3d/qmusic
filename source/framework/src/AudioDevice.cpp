#include <QDebug>
#include "portaudio.h"
#include "Application.h"
#include "AudioDevice.h"

#define BUFFER_SIZE (1024)

static int audioDeviceCallback(const void *pInputBuffer,
                               void *pOutputBuffer,
                               unsigned long framesPerBuffer,
                               const PaStreamCallbackTimeInfo *pTimeInfo,
                                PaStreamCallbackFlags statusFlags,
                                void *pData)
{
    AudioDevice *pAudioDevice = static_cast<AudioDevice*>(pData);

    const float* pIn = static_cast<const float*>(pInputBuffer);
    float *pOut = static_cast<float*>(pOutputBuffer);

    pAudioDevice->processAudio(pIn, pOut, framesPerBuffer);

    return 0;
}


AudioDevice::AudioDevice()
    : m_pStream(nullptr)
{
    int err = Pa_Initialize();
    if (err != paNoError) {
        qCritical() << "Unable initialize audio device:" << Pa_GetErrorText(err);
    }

    m_openDeviceInfo.index = paNoDevice;
    m_openDeviceInfo.name = "No Device";
    m_openDeviceInfo.nInputs = 0;
    m_openDeviceInfo.nOutputs = 0;
    m_openDeviceInfo.sampleRate = 0.0f;

    foreach(const Info &info, enumarate()) {
        qDebug() << info.name << "inputs:" << info.nInputs
                 << "outputs:" << info.nOutputs
                 << "sample rate:" << info.sampleRate;
    }
}

AudioDevice::~AudioDevice()
{
    Pa_Terminate();
}

QList<AudioDevice::Info> AudioDevice::enumarate() const
{
    QList<Info> devices;

    PaDeviceIndex n = Pa_GetDeviceCount();
    for (PaDeviceIndex i = 0; i < n; i++) {
        devices.append(getInfo(i));
    }

    return devices;
}

bool AudioDevice::open(int index, int nInputs, int nOutputs, double sampleRate)
{
    if (isOpen()) {
        if (!close()) {
            return false;
        }
    }

    PaStreamParameters inputParameters;
    inputParameters.device = index;
    inputParameters.channelCount = nInputs;
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(index)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    PaStreamParameters outputParameters;
    outputParameters.device = index;
    outputParameters.channelCount = nOutputs;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(index)->defaultLowInputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    m_openDeviceInfo = getInfo(index);
    m_openDeviceInfo.nInputs = nInputs;
    m_openDeviceInfo.nOutputs = nOutputs;
    m_openDeviceInfo.sampleRate = sampleRate;

    int err = Pa_OpenStream(&m_pStream,
                            &inputParameters,
                            &outputParameters,
                            sampleRate,
                            BUFFER_SIZE,
                            0, // paClipOff
                            audioDeviceCallback,
                            this
                           );

    if (err != paNoError) {
        qCritical() << "Unable to open audio device" << index
                    << Pa_GetErrorText(err);
        return false;
    }


    return true;
}

bool AudioDevice::close()
{
    if (!isOpen()) {
        return true;
    }

    int err = Pa_CloseStream(m_pStream);
    if (err != paNoError) {
        qCritical() << "Unable to close audio device" << Pa_GetErrorText(err);
        return false;
    }

    m_pStream = nullptr;
    return true;
}

bool AudioDevice::isOpen() const
{
    return m_pStream != nullptr;
}

bool AudioDevice::start()
{
    if (!isOpen()) {
        qCritical() << "Unable to start audio stream: device is closed";
        return false;
    }

    int err = Pa_StartStream(m_pStream);
    if (err != paNoError) {
        qCritical() << "Unable to start audio stream:" << Pa_GetErrorText(err);
        return false;
    }

    return true;
}

bool AudioDevice::stop()
{
    if (!isOpen()) {
        return true;
    }

    int err = Pa_StopStream(m_pStream);
    if (err != paNoError) {
        qCritical() << "Unable to stop audio sttream:" << Pa_GetErrorText(err);
        return false;
    }

    return true;
}

void AudioDevice::addListener(IAudioDeviceListener *pListener)
{
    Q_ASSERT(pListener != nullptr);
    m_listeners.append(pListener);
}

void AudioDevice::removeListener(IAudioDeviceListener *pListener)
{
    m_listeners.removeAll(pListener);
}

void AudioDevice::processAudio(const float *pInputBuffer, float *pOutputBuffer, long nSamples)
{
    foreach (IAudioDeviceListener *pListener, m_listeners) {
        pListener->processAudio(pInputBuffer, pOutputBuffer, nSamples);
    }
}

AudioDevice::Info AudioDevice::getInfo(int index) const
{
    Info devInfo;
    const PaDeviceInfo *pInfo = Pa_GetDeviceInfo(index);

    devInfo.index = index;
    devInfo.name = QString::fromLatin1(pInfo->name);
    devInfo.nInputs = pInfo->maxInputChannels;
    devInfo.nOutputs = pInfo->maxOutputChannels;
    devInfo.sampleRate = pInfo->defaultSampleRate;
    return devInfo;
}
