#include <QDebug>
#include <QThread>
#include "Application.h"
#include "Settings.h"
#include "ISignalChain.h"
#include "SpeakerThreadObject.h"
#include "Speaker.h"

const QColor cDefaultColor(240, 230, 210);

Speaker::Speaker(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pInputLeft = addInput("L", QVariant::Double);
    m_pInputRight = addInput("R", QVariant::Double);

    m_pThread = new QThread(this);
    m_pThreadObject = new SpeakerThreadObject(bufferSizeFropmSettings());
    m_pThreadObject->setInputPorts(m_pInputLeft, m_pInputRight);
    m_pThreadObject->moveToThread(m_pThread);
    m_pThread->start(QThread::TimeCriticalPriority);

    m_pLeftBuffer = nullptr;
    m_pRightBuffer = nullptr;

    Application::instance()->audioOutputDevice()->addListener(this);
}

Speaker::~Speaker()
{
    Application::instance()->audioOutputDevice()->removeListener(this);
    m_pThreadObject->stop();
    m_pThread->quit();
    delete m_pThreadObject;

    releaseBuffers();
}

QColor Speaker::color() const
{
    return cDefaultColor;
}

void Speaker::processAudio(const float *pInputBuffer, float *pOutputBuffer, long nSamples)
{
    Q_UNUSED(pInputBuffer);


    long length = leftBuffer()->availableToRead();
    length = qMin(length, rightBuffer()->availableToRead());
    length = qMin(length, nSamples);

    leftBuffer()->read(m_pLeftBuffer, length);
    rightBuffer()->read(m_pRightBuffer, length);

    long i = 0;

    while (i < length) {
        *pOutputBuffer++ = m_pLeftBuffer[i];
        *pOutputBuffer++ = m_pRightBuffer[i];
        ++i;
    }

    // If not enough data, fill with zeroes
    while (i < nSamples) {
        *pOutputBuffer++ = 0.0f;
        *pOutputBuffer++ = 0.0f;
        ++i;
    }
}

AudioBuffer* Speaker::leftBuffer() const
{
    return m_pThreadObject->leftChannelBuffer();
}

AudioBuffer* Speaker::rightBuffer() const
{
    return m_pThreadObject->rightChannelBuffer();
}

void Speaker::processStart()
{
    // Delete buffers in case the size is changed
    allocateBuffers();

    signalChain()->reset();
    m_pThreadObject->setSignalChain(signalChain());

    m_pThreadObject->start();
}

void Speaker::processStop()
{
    m_pThreadObject->stop();
}

void Speaker::process()
{
}

void Speaker::allocateBuffers()
{
    releaseBuffers();
    int bufferSize = bufferSizeFropmSettings();
    m_pLeftBuffer = new float[2 * bufferSize];
    m_pRightBuffer = new float[2 * bufferSize];
}

void Speaker::releaseBuffers()
{
    delete m_pLeftBuffer;
    delete m_pRightBuffer;
    m_pLeftBuffer = nullptr;
    m_pRightBuffer = nullptr;
}

int Speaker::bufferSizeFropmSettings()
{
    Settings settings;
    return settings.get(Settings::Setting_BufferSize).toInt();
}
