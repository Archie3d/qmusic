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

#ifndef AU_MIXER_H
#define AU_MIXER_H

#include <QList>
#include "AudioUnit.h"

class QtVariantProperty;

class Mixer : public AudioUnit
{
public:

    static const int cMinNumberOfInputs;
    static const int cMaxNumberOfInputs;

    Mixer(AudioUnitPlugin *pPlugin);
    ~Mixer();

    void createInputs(int nInputs);

    // ISerializable interface
    void serialize(QVariantMap &data, SerializationContext *pContext) const override;
    void deserialize(const QVariantMap &data, SerializationContext *pContext) override;

protected:

    void processStart() override;
    void processStop() override;
    void process() override;

private:

    void createProperties();

    QList<InputPort*> m_inputs;
    OutputPort *m_pOutput;
    float m_mixFactor;

    QtVariantProperty *m_pPropMixingFactor;
};

#endif // AU_ADDER_H

