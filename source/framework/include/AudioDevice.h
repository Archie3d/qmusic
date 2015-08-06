#ifndef AUDIODEVICE_H
#define AUDIODEVICE_H

#include <QList>
#include "FrameworkApi.h"

class QMUSIC_FRAMEWORK_API IAudioDeviceListener
{
public:

    virtual void processAudio(const float *pInputBuffer,
                              float *pOutputBuffer,
                              long nSamples) = 0;

    virtual ~IAudioDeviceListener() {}
};

typedef void PaStream;

/**
 * @brief Audio device based on portaudio
 */
class QMUSIC_FRAMEWORK_API AudioDevice
{
public:

    /// Device information
    struct Info {
        int index;      ///< Deice index.
        QString name;   ///< Device name.
        int nInputs;    ///< Number of input channels.
        int nOutputs;   ///< Number of output channels.
        double sampleRate;   ///< Sample rate.
    };

    /**
     * Create audio device but do not initialize it.
     */
    AudioDevice();
    ~AudioDevice();

    QList<Info> enumarate() const;

    bool open(int index,
              int nInputs,
              int nOutputs,
              double sampleRate,
              int bufferSize);

    bool close();

    bool isOpen() const;

    bool start();
    bool stop();

    void addListener(IAudioDeviceListener *pListener);
    void removeListener(IAudioDeviceListener *pListener);

    Info openDeviceInfo() const { return m_openDeviceInfo; }

    void processAudio(const float *pInputBuffer,
                      float *pOutputBuffer,
                      long nSamples);

private:

    Info getInfo(int index) const;

    PaStream *m_pStream;
    struct Info m_openDeviceInfo;
    QList<IAudioDeviceListener*> m_listeners;
};

#endif // AUDIODEVICE_H

