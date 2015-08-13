#include <chrono>
#include <QTimer>
#include "ISignalChain.h"
#include "AudioBuffer.h"
#include "SpeakerThreadObject.h"

SpeakerThreadObject::SpeakerThreadObject(long bufferSize, QObject *pParent)
    : QObject(pParent),
      m_bufferSize(bufferSize)
{
    m_pSignalChain = nullptr;
    m_pLeftBuffer = new AudioBuffer(bufferSize * 2);
    m_pRightBuffer = new AudioBuffer(bufferSize * 2);
    m_pLeftData = new float[bufferSize * 2];
    m_pRightData = new float[bufferSize * 2];

    m_started = false;
    m_dspLoad = 0.0f;

    connect(this, SIGNAL(started()), this, SLOT(generateSamples()), Qt::QueuedConnection);
    connect(this, SIGNAL(continueGenerateSamples()),
            this, SLOT(generateSamples()), Qt::QueuedConnection);
}

SpeakerThreadObject::~SpeakerThreadObject()
{
    delete m_pLeftBuffer;
    delete m_pRightBuffer;
    delete[] m_pLeftData;
    delete[] m_pRightData;
}

void SpeakerThreadObject::setSignalChain(ISignalChain *pSignalChain)
{
    Q_ASSERT(pSignalChain != nullptr);
    QMutexLocker lock(&m_mutex);
    m_pSignalChain = pSignalChain;
}

void SpeakerThreadObject::setInputPorts(InputPort *pLeft, InputPort *pRight)
{
    QMutexLocker lock(&m_mutex);
    m_pLeftChannelInput = pLeft;
    m_pRightChannelInput = pRight;
}

void SpeakerThreadObject::start()
{
    QMutexLocker lock(&m_mutex);
    m_started = true;
    m_firstBuffer = true;
    m_pLeftBuffer->clear();
    m_pRightBuffer->clear();
    m_dspLoad = 0.0;
    setDspLoad(0.0f);
    emit started();
}

void SpeakerThreadObject::stop()
{
    QMutexLocker lock(&m_mutex);
    m_started = false;
    setDspLoad(0.0f);
}

float SpeakerThreadObject::getNextLeftChannelSample()
{
    m_pLeftChannelInput->update();
    return m_pLeftChannelInput->value().asFloat;
}

float SpeakerThreadObject::getNextRightChannelSample()
{
    m_pRightChannelInput->update();
    return m_pRightChannelInput->value().asFloat;
}

void SpeakerThreadObject::generateSamples()
{
    // This method is always called from this object's thread
    if (!m_started) {
        return;
    }

    long available = m_pLeftBuffer->availableToWrite();
    available = qMin(available, m_pRightBuffer->availableToWrite());

    if (available < m_bufferSize / 2) {
        // If availability is low, trigger timer
        emit continueGenerateSamples();
    } else {

        double realTimeUs = m_pSignalChain->timeStep() * available * 1.0e6;

        auto startTime = std::chrono::high_resolution_clock::now();
        for (long i = 0; i < available; i++) {
            m_pSignalChain->prepareUpdate();
            m_pLeftData[i] = getNextLeftChannelSample();
            m_pRightData[i] = getNextRightChannelSample();
        }
        m_pLeftBuffer->write(m_pLeftData, available);
        m_pRightBuffer->write(m_pRightData, available);
        auto processingTime = std::chrono::high_resolution_clock::now() - startTime;
        double dspTimeUs = double(std::chrono::duration_cast<std::chrono::microseconds>(processingTime).count());

        float l = dspTimeUs / realTimeUs;


        setDspLoad(l);

        // Continue with the samples generation
        emit continueGenerateSamples();
    }

    if (m_firstBuffer) {
        m_firstBuffer = false;
        emit bufferReady();
    }
}

void SpeakerThreadObject::setDspLoad(float l)
{
    float dl = fabs(m_dspLoad - l);
    if (dl >= 0.05) {
        // Do some averaging to filter rapid changes
        m_dspLoad = m_dspLoad * 0.8 + l * 0.2;
        emit dspLoadChanged(m_dspLoad);
    }
}
