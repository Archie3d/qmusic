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

#ifndef AU_GENERATOR_H
#define AU_GENERATOR_H

#include "AudioUnit.h"

class QtVariantProperty;

/*
 *  Universal waveform generator
 */

class Generator : public AudioUnit
{
public:

    Generator(AudioUnitPlugin *pPlugin);
    ~Generator();

    QColor color() const override;

    // ISerializable interface
    void serialize(QVariantMap &data, SerializationContext *pContext) const override;
    void deserialize(const QVariantMap &data, SerializationContext *pContext) override;

protected:

    void processStart() override;
    void processStop() override;
    void process() override;
    void reset() override;

    void noteOnEvent(NoteOnEvent *pEvent) override;

private:

    void createProperties();
    void setValues();

    float m_phase;
    float m_dt;
    int m_waveform;
    bool m_bandlimit;
    bool m_trigger;

    InputPort *m_pInputFreq;
    OutputPort *m_pOutput;

    QtVariantProperty *m_pPropWaveform;
    QtVariantProperty *m_pPropBandPassLimit;
    QtVariantProperty *m_pPropTrigger;
};

#endif // AU_GENERATOR_H

