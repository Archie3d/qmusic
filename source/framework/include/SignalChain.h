#ifndef SIGNALCHAIN_H
#define SIGNALCHAIN_H

#include <QList>
#include "FrameworkApi.h"
#include "ISignalChain.h"

class QThread;
class IAudioUnit;

/**
 * @brief Signal chain.
 *
 * This class implements a signal chain.
 * This is a data model implementation, the view is implemented by
 * signal chain scene.
 * @see SignalChainScene
 */
class QMUSIC_FRAMEWORK_API SignalChain : public ISignalChain
{
public:

    static const QString UID;

    SignalChain();
    ~SignalChain();

    // ISignalChain interface
    void start() override;
    void stop() override;
    bool isStarted() const override { return m_started; }
    void reset() override;
    double timeStep() const override { return m_timeStep; }
    void setTimeStep(double dt) override { m_timeStep = dt; }
    double sampleRate() const override { return 1.0 / m_timeStep; }
    void addAudioUnit(IAudioUnit *pAudioUnit) override;
    void removeAudioUnit(IAudioUnit *pAudioUnit) override;
    void prepareUpdate() override;

    // ISerializable interface
    QString uid() const override final { return UID; }
    void serialize(QVariantMap &data, SerializationContext *pContext) const override;
    void deserialize(const QVariantMap &data, SerializationContext *pContext);
    static ISerializable* create() { return new SignalChain(); }

private:

    void startAudioDevices();
    void stopAudioDevices();
    void startAllAudioUnits();
    void stopAllAudioUnits();
    void resetAllAudioUnits();

    /// Current global time, s
    double m_timeStep;

    /// Signal chain processing has been started.
    bool m_started;

    /// Audio units in this chain.
    QList<IAudioUnit*> m_audioUnits;
};

#endif // SIGNALCHAIN_H

