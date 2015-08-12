#include <QDebug>
#include "portaudio.h"
#include "Application.h"
#include "Settings.h"
#include "AudioBuffer.h"
#include "ISignalChain.h"
#include "../include/Input.h"

const QColor cDefaultColor(210, 230, 240);

Input::Input(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pLeftBuffer = nullptr;
    m_pRightBuffer = nullptr;

    m_pOutputLeft = addOutput("L", Signal::Type_Float);
    m_pOutputRight = addOutput("R", Signal::Type_Float);

    m_pLeft = nullptr;
    m_pRight = nullptr;

    Application::instance()->audioInputDevice()->addListener(this);
}

Input::~Input()
{
    Application::instance()->audioInputDevice()->removeListener(this);

    releaseBuffers();
}

QColor Input::color() const
{
    return cDefaultColor;
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
        m_pLeft[i] = *pInputBuffer++;
        m_pRight[i] = *pInputBuffer++;
    }

    m_pLeftBuffer->write(m_pLeft, length);
    m_pRightBuffer->write(m_pRight, length);
}

void Input::processStart()
{
    allocateBuffers();

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

    m_pOutputLeft->setFloatValue(l);
    m_pOutputRight->setFloatValue(r);
}

void Input::allocateBuffers()
{
    Settings settings;

    releaseBuffers();

    bool ok = false;
    int bufferSize = settings.get(Settings::Setting_BufferSize).toInt(&ok);
    if (!ok || bufferSize <= 0) {
        bufferSize = 1024;
    }
    m_pLeftBuffer = new AudioBuffer(2 * bufferSize);
    m_pRightBuffer = new AudioBuffer(2 * bufferSize);
    m_pLeft = new float[2 * bufferSize];
    m_pRight = new float[2 * bufferSize];
}

void Input::releaseBuffers()
{
    delete m_pLeftBuffer;
    delete m_pRightBuffer;
    delete[] m_pLeft;
    delete[] m_pRight;
    m_pLeftBuffer = nullptr;
    m_pRightBuffer = nullptr;
    m_pLeft = nullptr;
    m_pRight = nullptr;
}
