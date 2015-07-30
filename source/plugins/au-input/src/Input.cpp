#include <QDebug>
#include "portaudio.h"
#include "Application.h"
#include "AudioBuffer.h"
#include "ISignalChain.h"
#include "../include/Input.h"

#define BUFFER_SIZE (1024)

float sLeft[BUFFER_SIZE];
float sRight[BUFFER_SIZE];

Input::Input(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pLeftBuffer = new AudioBuffer(BUFFER_SIZE * 2);
    m_pRightBuffer = new AudioBuffer(BUFFER_SIZE * 2);

    m_outputLeftPtr = OutputPortPtr(new OutputPort("L", QVariant::Double));
    m_outputRightPtr = OutputPortPtr(new OutputPort("R", QVariant::Double));

    addOutput(m_outputLeftPtr);
    addOutput(m_outputRightPtr);

    Application::instance()->audioDevice()->addListener(this);
}

Input::~Input()
{
    Application::instance()->audioDevice()->removeListener(this);

    delete m_pLeftBuffer;
    delete m_pRightBuffer;
}

void Input::processAudio(const float *pInputBuffer, float *pOutputBuffer, long nSamples)
{
    if (pInputBuffer == nullptr || nSamples <= 0) {
        // Nothing to process
        return;
    }

    long length = qMin(nSamples, m_pLeftBuffer->availableToWrite());
    length = qMin(length, m_pRightBuffer->availableToWrite());

    for (long i = 0; i < length; i++) {
        sLeft[i] = *pInputBuffer++;
        sRight[i] = *pInputBuffer++;
    }

    m_pLeftBuffer->write(sLeft, length);
    m_pRightBuffer->write(sRight, length);
}

void Input::processStart()
{
    m_pLeftBuffer->clear();
    m_pRightBuffer->clear();
}

void Input::processStop()
{
}

void Input::process()
{
    float l = 0.0f;
    float r = 0.0f;

    if (m_pLeftBuffer->availableToRead() > 0 && m_pRightBuffer->availableToRead() > 0) {
        m_pLeftBuffer->read(&l, 1);
        m_pRightBuffer->read(&r, 1);
    }

    m_outputLeftPtr->setValue(l);
    m_outputRightPtr->setValue(r);
}
