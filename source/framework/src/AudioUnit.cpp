#include <QtVariantPropertyManager>
#include <QtVariantProperty>
#include "AudioUnitPlugin.h"
#include "AudioUnit.h"

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

    foreach (InputPort *pInput, m_inputs) {
        pInput->update();
    }

    process();
    m_updated = true;
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

void AudioUnit::control(const QString &name, const QVariant &value)
{
    Q_UNUSED(name);
    Q_UNUSED(value);
    // Ignore controls by default.
}

InputPort *AudioUnit::addInput(const QString &name, QVariant::Type type)
{
    InputPort *pInput = new InputPort(name, type);
    addInput(pInput);
    return pInput;
}

void AudioUnit::addInput(InputPort *pInput)
{
    Q_ASSERT(pInput != nullptr);
    m_inputs.append(pInput);
}

QList<InputPort *> AudioUnit::inputs() const
{
    return m_inputs;
}

OutputPort *AudioUnit::addOutput(const QString &name, QVariant::Type type)
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

QList<OutputPort *> AudioUnit::outputs() const
{
    return m_outputs;
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
