#ifndef AUDIOSINK_H
#define AUDIOSINK_H

#include "AudioDevice.h"
#include "AudioUnit.h"

class AudioBuffer;
typedef void PaStream;

class Input : public AudioUnit,
              public IAudioDeviceListener
{
public:

    Input(AudioUnitPlugin *pPlugin);
    ~Input();

    void processAudio(const float *pInputBuffer, float *pOutputBuffer, long nSamples) override;

protected:

    void processStart();
    void processStop();
    void process();

private:

    AudioBuffer *m_pLeftBuffer;
    AudioBuffer *m_pRightBuffer;

    OutputPort *m_pOutputLeft;
    OutputPort *m_pOutputRight;
};

#endif // AUDIOSINK_H

