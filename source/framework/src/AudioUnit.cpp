#include <QtVariantPropertyManager>
#include <QtVariantProperty>
#include "SerializationContext.h"
#include "SignalChain.h"
#include "AudioUnitPlugin.h"
#include "AudioUnit.h"

/// Default color used for audio units representation
const QColor cDefaultColor(220, 220, 220);

AudioUnit::AudioUnit(AudioUnitPlugin *pPlugin, QObject *pParent)
    : m_pSignalChain(nullptr),
      m_pPlugin(pPlugin),
      m_inputs(),
      m_outputs(),
      m_started(false)
{
    Q_ASSERT(pPlugin != nullptr);

    m_pPropertyManager = new QtVariantPropertyManager();
    m_pRootProperty = m_pPropertyManager->addProperty(QtVariantPropertyManager::groupTypeId());
}

AudioUnit::~AudioUnit()
{
    if (m_pSignalChain != nullptr) {
        // Detach audio unit
        m_pSignalChain->removeAudioUnit(this);
    }

    // Delete ports
    qDeleteAll(m_inputs);
    qDeleteAll(m_outputs);

    delete m_pPropertyManager;
}

void AudioUnit::prepareUpdate()
{
    m_updated = false;
}

void AudioUnit::update()
{
    if (!isStarted()) {
        return;
    }

    if (m_updated) {
        // Already updated.
        return;
    }

    // Activate already the updated flag to prevent loop recursion
    m_updated = true;

    foreach (InputPort *pInput, m_inputs) {
        pInput->update();
    }

    process();
}

void AudioUnit::start()
{
    if (isStarted()) {
        return;
    }

    processStart();
    m_started = true;
}

void AudioUnit::stop()
{
    if (!isStarted()) {
        return;
    }

    processStop();
    m_started = false;
}

QColor AudioUnit::color() const
{
    return cDefaultColor;
}

InputPort *AudioUnit::addInput(const QString &name, Signal::Type type)
{
    InputPort *pInput = new InputPort(name, type);
    addInput(pInput);
    return pInput;
}

void AudioUnit::addInput(InputPort *pInput)
{
    Q_ASSERT(pInput != nullptr);
    pInput->setAudioUnit(this);
    m_inputs.append(pInput);
}

OutputPort *AudioUnit::addOutput(const QString &name, Signal::Type type)
{
    OutputPort *pOutput = new OutputPort(name, type);
    addOutput(pOutput);
    return pOutput;
}

void AudioUnit::addOutput(OutputPort *pOutput)
{
    Q_ASSERT(pOutput != nullptr);
    pOutput->setAudioUnit(this);
    m_outputs.append(pOutput);
}

void AudioUnit::removeAllInputs()
{
    qDeleteAll(m_inputs);
    m_inputs.clear();
}

void AudioUnit::removeAllOutputs()
{
    qDeleteAll(m_outputs);
    m_outputs.clear();
}

void AudioUnit::removeAllPorts()
{
    removeAllInputs();
    removeAllOutputs();
}

QString AudioUnit::uid() const
{
    return m_pPlugin->uid();
}

void AudioUnit::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
}

void AudioUnit::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
}
