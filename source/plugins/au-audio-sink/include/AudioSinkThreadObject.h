#ifndef AUDIOSINKTHREADOBJECT_H
#define AUDIOSINKTHREADOBJECT_H

#include <QObject>
#include <QMutex>
#include "InputPort.h"

class ISignalChain;
class AudioBuffer;

/**
 * This object is used to trigger the signal chain and it should reside in a
 * seperate thread.
 */
class AudioSinkThreadObject : public QObject
{
    Q_OBJECT
public:

    AudioSinkThreadObject(long bufferSize, QObject *pParent = nullptr);
    ~AudioSinkThreadObject();

    AudioBuffer* leftChannelBuffer() const { return m_pLeftBuffer; }
    AudioBuffer* rightChannelBuffer() const { return m_pRightBuffer; }

    void setSignalChain(ISignalChain *pSignalChain);
    void setInputPorts(InputPortPtr left, InputPortPtr right);

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

    InputPortPtr m_leftChannelInputPtr;
    InputPortPtr m_rightChannelInputPtr;
};

#endif // AUDIOSINKTHREADOBJECT_H

