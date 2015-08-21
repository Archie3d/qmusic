/*
                          qmusic

    Copyright (C) 2015 Arthur Benilov,
    arthur.benilov@gmail.com

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.
*/

#ifndef SIGNALCHAIN_H
#define SIGNALCHAIN_H

#include <QList>
#include "FrameworkApi.h"
#include "ISignalChain.h"

class QThread;
class IAudioUnit;
class SignalChainEvent;

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
    void enable(bool v) override;
    bool isEnabled() const override { return m_enabled; }
    void sendEvent(SignalChainEvent *pEvent) override;
    void postEvent(SignalChainEvent *pEvent) override;
    void processEvents() override;
    float timeStep() const override { return m_timeStep; }
    void setTimeStep(double dt) override { m_timeStep = dt; }
    float sampleRate() const override { return 1.0f / m_timeStep; }
    void addAudioUnit(IAudioUnit *pAudioUnit) override;
    void removeAudioUnit(IAudioUnit *pAudioUnit) override;
    QList<IAudioUnit*> audioUnits() const override { return m_audioUnits; }
    void prepareUpdate() override;
    QList<ISignalChain*> clone(int instances = 1) override;

    // ISerializable interface
    QString uid() const override final { return UID; }
    void serialize(QVariantMap &data, SerializationContext *pContext) const override;
    void deserialize(const QVariantMap &data, SerializationContext *pContext);
    static ISerializable* create() { return new SignalChain(); }

private:

    void startAllAudioUnits();
    void stopAllAudioUnits();
    void resetAllAudioUnits();
    void clearEventsQueue();
    void processEvent(SignalChainEvent *pEvent);

    /// Current global time, s
    float m_timeStep;

    /// Signal chain processing has been started.
    bool m_started;

    /// Whether the signal chain is enabled.
    bool m_enabled;

    /// Counter used to decimate events processing.
    int m_updateEventsCounter;

    /// Audio units in this chain.
    QList<IAudioUnit*> m_audioUnits;

    /// Events queue.
    QList<SignalChainEvent*> m_events;
};

#endif // SIGNALCHAIN_H

