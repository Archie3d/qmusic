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
#include "ISignalChainSceneContainer.h"

class QtVariantProperty;
class QGraphicsSimpleTextItem;


class PolyphonicContainer : public AudioUnit,
                                   ISignalChainSceneContainer
{
public:

    PolyphonicContainer(AudioUnitPlugin *pPlugin);
    ~PolyphonicContainer();

    void setSignalChainScene(SignalChainScene *pScene) override;
    SignalChainScene* signalChainScene() const override { return m_pSignalChainScene; }

    void handleEvent(SignalChainEvent *pEvent) override;

protected:

    void processStart();
    void processStop();
    void process();
    void reset();

    QColor color() const override;
    int flags() const;
    QGraphicsItem* graphicsItem() override;

    // ISerializable interface
    void serialize(QVariantMap &data, SerializationContext *pContext) const;
    void deserialize(const QVariantMap &data, SerializationContext *pContext);

private:

    void createProperties();
    void createVoices(int n);
    void createPorts();
    void prepareVoicesUpdate();
    void manageVoices();
    void freeAllVoices();

    void allocateVoices();
    void releaseVoices();

    void updateView();

    /**
     * Pick a voice by removing it from the list of
     * free voices.
     * @return
     */
    ISignalChain* pickFreeVoice();

    SignalChainScene *m_pSignalChainScene;

    QList<InputPort*> m_inputs;
    QList<OutputPort*> m_outputs;
    QList<IAudioUnit*> m_exposeOutputAudioUnits;

    /// List of cloned signal chains
    QList<ISignalChain*> m_voices;

    /// List of available voices to play
    QList<ISignalChain*> m_freeVoices;

    /// Voices currently playing.
    QMap<int, ISignalChain*> m_busyVoices;

    QGraphicsSimpleTextItem *m_pLabelItem;
    QtVariantProperty *m_pPropLabel;
    QtVariantProperty *m_pPropNumberOfVoices;
};

#endif // AU_POLY_CONTAINER_H

