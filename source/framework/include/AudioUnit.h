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

class QMUSIC_FRAMEWORK_API AudioUnit : public IAudioUnit
{
    friend class SignalChain;
public:

    AudioUnit(AudioUnitPlugin *pPlugin, QObject *pParent = nullptr);
    ~AudioUnit();

    void prepareUpdate() override final;
    void update() override final;
    void start() override final;
    void stop() override final;
    bool isStarted() const override final { return m_started; }   
    void reset() override {}
    QGraphicsItem* graphicsItem() override { return nullptr; }
    int flags() const override { return Flag_NoFlags; }
    void control(const QString &name, const QVariant &value) override;

    AudioUnitPlugin* plugin() const { return m_pPlugin; }

    InputPort* addInput(const QString &name, QVariant::Type type);
    void addInput(InputPort *pInput);
    const QList<InputPort*>& inputs() const { return m_inputs; }

    OutputPort* addOutput(const QString &name, QVariant::Type type);
    void addOutput(OutputPort *pOutput);
    const QList<OutputPort*>& outputs() const { return m_outputs; }

    void removeAllInputs();
    void removeAllOutputs();
    void removeAllPorts();

    ISignalChain* signalChain() const { return m_pSignalChain; }

    QtVariantPropertyManager* propertyManager() const { return m_pPropertyManager; }
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

