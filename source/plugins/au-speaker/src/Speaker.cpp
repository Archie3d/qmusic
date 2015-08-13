#include <QDebug>
#include <QThread>
#include "Application.h"
#include "Settings.h"
#include "ISignalChain.h"
#include "MainWindow.h"
#include "SpeakerThreadObject.h"
#include "Speaker.h"

const QColor cDefaultColor(240, 230, 210);

Speaker::Speaker(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pInputLeft = addInput("L", Signal::Type_Float);
    m_pInputRight = addInput("R", Signal::Type_Float);

    m_pThread = new QThread(this);
    m_pThreadObject = new SpeakerThreadObject(bufferSizeFromSettings());
    m_pThreadObject->setInputPorts(m_pInputLeft, m_pInputRight);
    m_pThreadObject->moveToThread(m_pThread);
    m_pThread->start(QThread::IdlePriority);

    m_pLeftBuffer = nullptr;
    m_pRightBuffer = nullptr;

    Application::instance()->audioOutputDevice()->addListener(this);

    // Connect DSL load signal with the main GUI
    MainWindow *pMainWindow = dynamic_cast<MainWindow*>(Application::instance()->mainWindow());
    if (pMainWindow != nullptr) {
        QObject::connect(m_pThreadObject, SIGNAL(dspLoadChanged(float)),
                         pMainWindow, SLOT(updateDspLoad(float)), Qt::QueuedConnection);
    }
}

Speaker::~Speaker()
{
    Application::instance()->audioOutputDevice()->removeListener(this);
    m_pThreadObject->stop();
    m_pThread->quit();
    m_pThread->wait(3000);
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

    m_pThread->setPriority(QThread::TimeCriticalPriority);
    m_pThreadObject->start();        
}

void Speaker::processStop()
{
    m_pThreadObject->stop();
    m_pThread->setPriority(QThread::IdlePriority);
}

void Speaker::process()
{
}

void Speaker::allocateBuffers()
{
    releaseBuffers();
    int bufferSize = bufferSizeFromSettings();
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

int Speaker::bufferSizeFromSettings()
{
    Settings settings;
    return settings.get(Settings::Setting_BufferSize).toInt();
}
