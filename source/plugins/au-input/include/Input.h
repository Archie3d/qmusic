#ifndef AU_INPUT_H
#define AU_INPUT_H

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

    QColor color() const override;

    void processAudio(const float *pInputBuffer, float *pOutputBuffer, long nSamples) override;

protected:

    void processStart();
    void processStop();
    void process();

private:

    void allocateBuffers();
    void releaseBuffers();

    AudioBuffer *m_pLeftBuffer;
    AudioBuffer *m_pRightBuffer;

    OutputPort *m_pOutputLeft;
    OutputPort *m_pOutputRight;

    float *m_pLeft;
    float *m_pRight;
};

#endif // AU_INPUT_H

