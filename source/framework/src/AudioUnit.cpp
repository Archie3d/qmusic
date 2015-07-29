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
    // Clear audio unit association with output ports
    foreach (const OutputPortPtr &outputPtr, m_outputs) {
        outputPtr->setAudioUnit(nullptr);
    }
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

    foreach (const InputPortPtr inputPtr, m_inputs) {
        inputPtr->update();
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

InputPortPtr AudioUnit::addInput(const QString &name, QVariant::Type type)
{
    InputPortPtr inputPtr(new InputPort(name, type));
    addInput(inputPtr);
    return inputPtr;
}

void AudioUnit::addInput(const InputPortPtr &inputPtr)
{
    Q_ASSERT(!inputPtr.isNull());
    m_inputs.append(inputPtr);
}

QList<InputPortPtr> AudioUnit::inputs() const
{
    return m_inputs;
}

OutputPortPtr AudioUnit::addOutput(const QString &name, QVariant::Type type)
{
    OutputPortPtr outputPtr(new OutputPort(name, type));
    addOutput(outputPtr);
    return outputPtr;
}

void AudioUnit::addOutput(const OutputPortPtr &outputPtr)
{
    Q_ASSERT(!outputPtr.isNull());
    outputPtr->setAudioUnit(this);
    m_outputs.append(outputPtr);
}

QList<OutputPortPtr> AudioUnit::outputs() const
{
    return m_outputs;
}

ControlInputPortPtr AudioUnit::addControlInput(const QString &name, QVariant::Type type)
{
    ControlInputPortPtr inputPtr(new ControlInputPort(this, name, type));
    addControlInput(inputPtr);
    return inputPtr;
}

void AudioUnit::addControlInput(const ControlInputPortPtr &inputPtr)
{
    Q_ASSERT(!inputPtr.isNull());
    inputPtr->setAudioUnit(this);
    m_controlInputs.append(inputPtr);
}

QList<ControlInputPortPtr> AudioUnit::controlInputs() const
{
    return m_controlInputs;
}

ControlOutputPortPtr AudioUnit::addControlOutput(const QString &name, QVariant::Type type)
{
    ControlOutputPortPtr outputPtr(new ControlOutputPort(name, type));
    addControlOutput(outputPtr);
    return outputPtr;
}

void AudioUnit::addControlOutput(const ControlOutputPortPtr &outputPtr)
{
    Q_ASSERT(!outputPtr.isNull());
    m_controlOutputs.append(outputPtr);
}

QList<ControlOutputPortPtr> AudioUnit::controlOutputs() const
{
    return m_controlOutputs;
}

void AudioUnit::removeAllInputs()
{
    m_inputs.clear();
}

void AudioUnit::removeAllOutputs()
{
    foreach (const OutputPortPtr &outputPtr, m_outputs) {
        outputPtr->setAudioUnit(nullptr);
    }
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
