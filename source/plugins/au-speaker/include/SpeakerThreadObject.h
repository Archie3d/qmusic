#ifndef SPEAKERTHREADOBJECT_H
#define SPEAKERTHREADOBJECT_H

#include <QObject>
#include <QMutex>
#include "InputPort.h"

class ISignalChain;
class AudioBuffer;

/**
 * This object is used to trigger the signal chain and it should reside in a
 * seperate thread.
 */
class SpeakerThreadObject : public QObject
{
    Q_OBJECT
public:

    SpeakerThreadObject(long bufferSize, QObject *pParent = nullptr);
    ~SpeakerThreadObject();

    AudioBuffer* leftChannelBuffer() const { return m_pLeftBuffer; }
    AudioBuffer* rightChannelBuffer() const { return m_pRightBuffer; }

    void setSignalChain(ISignalChain *pSignalChain);
    void setInputPorts(InputPort *pLeft, InputPort *pRight);

    float dspLoad() const { return m_dspLoad; }

public slots:

    void start();
    void stop();

    void signalUpdateOver();

signals:

    void started();
    void continueGenerateSamples();
    void bufferReady();

    /**
     * Notify DSP load update.
     * @param l
     */
    void dspLoadChanged(float l);

    void signalChanged(const float *s, int size);

private slots:

    float getNextLeftChannelSample();
    float getNextRightChannelSample();

    void generateSamples();

private:

    void setDspLoad(float l);

    void requestSignalUpdate();
    bool isSignalUpdateRequested() const { return m_singnalUpdateRequested; }

    QMutex m_mutex; ///< Protective mutex.

    ISignalChain *m_pSignalChain;

    long m_bufferSize;
    AudioBuffer *m_pLeftBuffer;
    AudioBuffer *m_pRightBuffer;
    float *m_pLeftData;
    float *m_pRightData;

    bool m_started;
    bool m_firstBuffer;

    /// Processing load, [0..1]
    float m_dspLoad;

    /// Signal waveform used for spectrum update
    float *m_pSignalBuffer;
    int m_signalIndex;
    bool m_singnalUpdateRequested;

    InputPort *m_pLeftChannelInput;
    InputPort *m_pRightChannelInput;
};

#endif // SPEAKERTHREADOBJECT_H

