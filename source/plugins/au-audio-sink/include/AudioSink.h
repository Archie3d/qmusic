#ifndef AUDIOSINK_H
#define AUDIOSINK_H

#include <QObject>
#include "AudioBuffer.h"
#include "AudioDevice.h"
#include "AudioUnit.h"

class QTimer;
class QThread;
class AudioSinkThreadObject;

class AudioSink : public QObject,
                  public AudioUnit,
                  public IAudioDeviceListener
{
    Q_OBJECT
public:

    enum Channel {
        Channel_Left,
        Channel_Right
    };

    AudioSink(AudioUnitPlugin *pPlugin);
    ~AudioSink();

    void processAudio(const float *pInputBuffer, float *pOutputBuffer, long nSamples) override;

    AudioBuffer* leftBuffer() const;
    AudioBuffer* rightBuffer() const;

protected:

    void processStart();
    void processStop();
    void process();

private slots:

    void startPlayback();

private:

    InputPortPtr m_inputLeftPtr;
    InputPortPtr m_inputRightPtr;
    QThread *m_pThread;
    AudioSinkThreadObject *m_pThreadObject;
};

#endif // AUDIOSINK_H

