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

#ifndef AU_STK_CLARINET_H
#define AU_STK_CLARINET_H

#include "AudioUnit.h"

class QtVariantProperty;

namespace stk {
    class Clarinet;
}

class StkClarinet : public AudioUnit
{
public:

    // Control messages
    enum {
        Ctrl_ReedStiffness = 2,
        Ctrl_NoiseGain = 4,
        Ctrl_VibratoFrequency = 11,
        Ctrl_VibratoGain = 1,
        Ctrl_BreathPressure = 128
    };

    StkClarinet(AudioUnitPlugin *pPlugin);
    ~StkClarinet();

    QColor color() const override { return QColor(250, 240, 255); }

    // ISerializable interface
    void serialize(QVariantMap &data, SerializationContext *pContext) const override;
    void deserialize(const QVariantMap &data, SerializationContext *pContext) override;

protected:

    void processStart() override;
    void processStop() override;
    void process() override;
    void reset() override;

    void noteOnEvent(NoteOnEvent *pEvent) override;
    void noteOffEvent(NoteOffEvent *pEvent) override;

private:

    void createProperties();

    InputPort *m_pInputFreq;
    InputPort *m_pInputVelocity;
    InputPort *m_pInputBreath;

    OutputPort *m_pOutput;

    int m_note;

    QtVariantProperty *m_pPropReedStiffness;
    QtVariantProperty *m_pPropNoiseGain;

    stk::Clarinet *m_pClarinet;
};

#endif // AU_STK_CLARINET_H

