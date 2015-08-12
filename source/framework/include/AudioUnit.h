#ifndef AUDIOUNIT_H
#define AUDIOUNIT_H

#include <QMap>
#include "FrameworkApi.h"
#include "IAudioUnit.h"
#include "InputPort.h"
#include "OutputPort.h"

class ISignalChain;
class QtVariantPropertyManager;
class QtVariantProperty;
class QtProperty;
class AudioUnitPlugin;

/**
 * @brief Abstract implementation of IAudioUnit interface.
 *
 * This is an abstract implementation that handles signal propagation
 * and chain update.
 *
 * This implementation also defines the audio unit properties container.
 * Though an audio unit does not have to define any properties, a root (hidden)
 * property node is always created.
 *
 * An audio unit must handle its own serialization (icluding properties).
 */
class QMUSIC_FRAMEWORK_API AudioUnit : public IAudioUnit
{
    friend class SignalChain;
public:

    AudioUnit(AudioUnitPlugin *pPlugin, QObject *pParent = nullptr);
    ~AudioUnit();

    // IAudioUnit interface
    void prepareUpdate() override final;
    void update() override final;
    void start() override final;
    void stop() override final;
    bool isStarted() const override final { return m_started; }   
    void reset() override {}
    QGraphicsItem* graphicsItem() override { return nullptr; }
    QColor color() const override;
    int flags() const override { return Flag_NoFlags; }

    /**
     * Returns pointer to the audio unit plugin.
     * @return
     */
    AudioUnitPlugin* plugin() const { return m_pPlugin; }

    /**
     * Register a new input port.
     * New input port will be added to this audio unit. The port will be destroyed together
     * with this audio unit object.
     * @param name Port name.
     * @param type Port data type.
     * @return Pointer to created port.
     */
    InputPort* addInput(const QString &name, Signal::Type type);

    /**
     * Add an input port.
     * The pointer will be retained.
     * @param pInput Pointer to the input port to be added.
     */
    void addInput(InputPort *pInput);

    /**
     * Returns the list of all input ports.
     * @return
     */
    const QList<InputPort*>& inputs() const { return m_inputs; }

    /**
     * Register a new output port.
     * New output port will be added to this audio unit. The port will be destroyed together
     * with this audio unit object.
     * @param name Port name.
     * @param type Port data type.
     * @return Pointer to created port.
     */
    OutputPort* addOutput(const QString &name, Signal::Type type);

    /**
     * Add an output port.
     * The pointer will be retained.
     * @param pOutput Pointer to the output port to be added.
     */
    void addOutput(OutputPort *pOutput);

    /**
     * Returns the list of all output ports.
     * @return
     */
    const QList<OutputPort*>& outputs() const { return m_outputs; }

    // Remove ports
    void removeAllInputs();
    void removeAllOutputs();
    void removeAllPorts();

    /**
     * Returns pointer to a signal chain this audio unit is associated with.
     * @return
     */
    ISignalChain* signalChain() const { return m_pSignalChain; }

    /**
     * Returns pointer to this audio unit properties manager.
     * @return
     */
    QtVariantPropertyManager* propertyManager() const { return m_pPropertyManager; }

    /**
     * Returns pointer to the root property of this audio unit.
     * @return
     */
    QtVariantProperty* rootProperty() const { return m_pRootProperty; }

    // ISerializable interface
    QString uid() const override final;
    void serialize(QVariantMap &data, SerializationContext *pContext) const override;
    void deserialize(const QVariantMap &data, SerializationContext *pContext) override;

protected:

    /**
     * @brief Notify processing start.
     * This method is called upon audio unit start.
     */
    virtual void processStart() {}

    /**
     * @brief Notify processing stop.
     * This method is called upon audio unit stop.
     */
    virtual void processStop() {}

    /**
     * @brief Perform the actual processing.
     *
     * Audio unit processing is invoked upon update.
     * The process method normally computes and assignes outputs
     * based on input port values.
     */
    virtual void process() = 0;

private:

    /**
     * @brief Assign signal chain to this audio unit.
     * @param pSignalChain Pointer to a signal chain this unit is put to.
     */
    void setSignalChain(ISignalChain *pSignalChain) { m_pSignalChain = pSignalChain; }

    /// Pointer to corresponding plugin
    AudioUnitPlugin *m_pPlugin;

    /// Pointer to signal chain this unit belongs to.
    ISignalChain *m_pSignalChain;

    /// Input ports.
    QList<InputPort*> m_inputs;

    /// Output ports.
    QList<OutputPort*> m_outputs;

    /// Flag telling that this unit has been already updated.
    bool m_updated;

    /// Properties container.
    QtVariantPropertyManager *m_pPropertyManager;
    QtVariantProperty *m_pRootProperty;

    /// Whether the unit processing has been started.
    bool m_started;
};

#endif // AUDIOUNIT_H

