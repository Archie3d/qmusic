#include <QTimer>
#include "ISignalChain.h"
#include "AudioBuffer.h"
#include "AudioSinkThreadObject.h"

AudioSinkThreadObject::AudioSinkThreadObject(long bufferSize, QObject *pParent)
    : QObject(pParent),
      m_bufferSize(bufferSize)
{
    m_pSignalChain = nullptr;
    m_pLeftBuffer = new AudioBuffer(bufferSize * 2);
    m_pRightBuffer = new AudioBuffer(bufferSize * 2);
    m_pLeftData = new float[bufferSize * 2];
    m_pRightData = new float[bufferSize * 2];

    m_started = false;

    connect(this, SIGNAL(started()), this, SLOT(generateSamples()), Qt::QueuedConnection);
    connect(this, SIGNAL(continueGenerateSamples()),
            this, SLOT(generateSamples()), Qt::QueuedConnection);
}

AudioSinkThreadObject::~AudioSinkThreadObject()
{
    delete m_pLeftBuffer;
    delete m_pRightBuffer;
    delete[] m_pLeftData;
    delete[] m_pRightData;
}

void AudioSinkThreadObject::setSignalChain(ISignalChain *pSignalChain)
{
    Q_ASSERT(pSignalChain != nullptr);
    QMutexLocker lock(&m_mutex);
    m_pSignalChain = pSignalChain;
}

void AudioSinkThreadObject::setInputPorts(InputPort *pLeft, InputPort *pRight)
{
    QMutexLocker lock(&m_mutex);
    m_pLeftChannelInput = pLeft;
    m_pRightChannelInput = pRight;
}

void AudioSinkThreadObject::start()
{
    QMutexLocker lock(&m_mutex);
    m_started = true;
    m_firstBuffer = true;
    m_pLeftBuffer->clear();
    m_pRightBuffer->clear();
    emit started();
}

void AudioSinkThreadObject::stop()
{
    QMutexLocker lock(&m_mutex);
    m_started = false;
}

float AudioSinkThreadObject::getNextLeftChannelSample()
{
    m_pLeftChannelInput->update();
    return m_pLeftChannelInput->value().toDouble();
}

float AudioSinkThreadObject::getNextRightChannelSample()
{
    m_pRightChannelInput->update();
    return m_pRightChannelInput->value().toDouble();
}

void AudioSinkThreadObject::generateSamples()
{
    // This method is always called from this object's thread
    if (!m_started) {
        return;
    }

    long available = m_pLeftBuffer->availableToWrite();
    available = qMin(available, m_pRightBuffer->availableToWrite());

    if (available < m_bufferSize / 2) {
        // If availability is low, trigger timer
        QTimer::singleShot(1, this, SLOT(generateSamples()));
    } else {
        for (long i = 0; i < available; i++) {
            m_pSignalChain->prepareUpdate();
            m_pLeftData[i] = getNextLeftChannelSample();
            m_pRightData[i] = getNextRightChannelSample();
        }
        m_pLeftBuffer->write(m_pLeftData, available);
        m_pRightBuffer->write(m_pRightData, available);

        // Continue with the samples generation
        emit continueGenerateSamples();
    }

    if (m_firstBuffer) {
        m_firstBuffer = false;
        emit bufferReady();
    }
}
