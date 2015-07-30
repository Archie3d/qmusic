#include <QDebug>
#include <QThread>
#include "Application.h"
#include "ISignalChain.h"
#include "AudioSinkThreadObject.h"
#include "AudioSink.h"

#define BUFFER_SIZE (1024)

// TODO: Move these buffers into class
static float sLeftBuffer[2*BUFFER_SIZE];
static float sRightBuffer[2*BUFFER_SIZE];


AudioSink::AudioSink(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pInputLeft = addInput("L", QVariant::Double);
    m_pInputRight = addInput("R", QVariant::Double);

    m_pThread = new QThread(this);
    m_pThreadObject = new AudioSinkThreadObject(BUFFER_SIZE);
    m_pThreadObject->setInputPorts(m_pInputLeft, m_pInputRight);
    m_pThreadObject->moveToThread(m_pThread);
    m_pThread->start(QThread::TimeCriticalPriority);

    Application::instance()->audioDevice()->addListener(this);
    connect(m_pThreadObject, SIGNAL(bufferReady()), this, SLOT(startPlayback()));
}

AudioSink::~AudioSink()
{
    Application::instance()->audioDevice()->removeListener(this);
    m_pThreadObject->stop();
    m_pThread->quit();
    delete m_pThreadObject;
}

void AudioSink::processAudio(const float *pInputBuffer, float *pOutputBuffer, long nSamples)
{
    Q_UNUSED(pInputBuffer);


    long length = leftBuffer()->availableToRead();
    length = qMin(length, rightBuffer()->availableToRead());
    length = qMin(length, nSamples);

    leftBuffer()->read(sLeftBuffer, length);
    rightBuffer()->read(sRightBuffer, length);

    long i = 0;

    while (i < length) {
        *pOutputBuffer++ = sLeftBuffer[i];
        *pOutputBuffer++ = sRightBuffer[i];
        ++i;
    }

    // If not enough data, fill with zeroes
    while (i < nSamples) {
        *pOutputBuffer++ = 0.0f;
        *pOutputBuffer++ = 0.0f;
        ++i;
    }
}

AudioBuffer* AudioSink::leftBuffer() const
{
    return m_pThreadObject->leftChannelBuffer();
}

AudioBuffer* AudioSink::rightBuffer() const
{
    return m_pThreadObject->rightChannelBuffer();
}

void AudioSink::processStart()
{
    signalChain()->reset();
    m_pThreadObject->setSignalChain(signalChain());

    m_pThreadObject->start();
}

void AudioSink::processStop()
{
    m_pThreadObject->stop();
}

void AudioSink::process()
{
}

void AudioSink::startPlayback()
{
    //Application::instance()->audioDevice()->start();
}
