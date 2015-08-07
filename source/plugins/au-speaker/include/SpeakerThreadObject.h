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

public slots:

    void start();
    void stop();

signals:

    void started();
    void continueGenerateSamples();
    void bufferReady();

private slots:

    float getNextLeftChannelSample();
    float getNextRightChannelSample();

    void generateSamples();

private:

    QMutex m_mutex; ///< Protective mutex.

    ISignalChain *m_pSignalChain;

    long m_bufferSize;
    AudioBuffer *m_pLeftBuffer;
    AudioBuffer *m_pRightBuffer;
    float *m_pLeftData;
    float *m_pRightData;

    bool m_started;
    bool m_firstBuffer;

    InputPort *m_pLeftChannelInput;
    InputPort *m_pRightChannelInput;
};

#endif // SPEAKERTHREADOBJECT_H

