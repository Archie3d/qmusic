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

#ifndef AU_POLY_CONTAINER_H
#define AU_POLY_CONTAINER_H

#include "AudioUnit.h"

class QtVariantProperty;
class QGraphicsSimpleTextItem;
class SignalChainScene;

class PolyphonicContainer : public AudioUnit
{
public:

    PolyphonicContainer(AudioUnitPlugin *pPlugin);
    ~PolyphonicContainer();

    void setSignalChainScene(SignalChainScene *pScene);

protected:

    void processStart();
    void processStop();
    void process();
    void reset();

    QColor color() const override;
    int flags() const;

    // ISerializable interface
    void serialize(QVariantMap &data, SerializationContext *pContext) const;
    void deserialize(const QVariantMap &data, SerializationContext *pContext);

private:

    void createProperties();
    void createVoices();
    void createPorts();
    void prepareVoicesUpdate();

    SignalChainScene *m_pSignalChainScene;

    QList<InputPort*> m_inputs;
    QList<OutputPort*> m_outputs;
    QList<IAudioUnit*> m_exposeOutputAudioUnits;

    /// List of cloned signal chains
    QList<ISignalChain*> m_voices;
};

#endif // AU_POLY_CONTAINER_H

