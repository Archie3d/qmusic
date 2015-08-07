#ifndef AU_SPEAKER_H
#define AU_SPEAKER_H

#include <QObject>
#include "AudioBuffer.h"
#include "AudioDevice.h"
#include "AudioUnit.h"

class QTimer;
class QThread;
class SpeakerThreadObject;

class Speaker : public QObject,
                public AudioUnit,
                public IAudioDeviceListener
{
    Q_OBJECT
public:

    enum Channel {
        Channel_Left,
        Channel_Right
    };

    Speaker(AudioUnitPlugin *pPlugin);
    ~Speaker();

    QColor color() const override;

    void processAudio(const float *pInputBuffer, float *pOutputBuffer, long nSamples) override;

    AudioBuffer* leftBuffer() const;
    AudioBuffer* rightBuffer() const;

protected:

    void processStart();
    void processStop();
    void process();

private:

    void allocateBuffers();
    void releaseBuffers();
    static int bufferSizeFropmSettings();

    InputPort *m_pInputLeft;
    InputPort *m_pInputRight;
    QThread *m_pThread;
    SpeakerThreadObject *m_pThreadObject;

    float *m_pLeftBuffer;
    float *m_pRightBuffer;
};

#endif // AU_SPEAKER_H

